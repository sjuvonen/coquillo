#include <taglib/fileref.h>

#include <taglib/flacfile.h>
#include <taglib/mpegfile.h>
#include <taglib/oggflacfile.h>
#include <taglib/vorbisfile.h>

#include <taglib/mp4tag.h>
#include <taglib/xiphcomment.h>
#include <taglib/id3v1tag.h>
#include <taglib/id3v2tag.h>

#include <taglib/attachedpictureframe.h>
#include <taglib/textidentificationframe.h>
#include <taglib/urllinkframe.h>

#include <QBuffer>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QImageWriter>
#include <QSettings>

#include "MetaDataWriter.h"

extern volatile bool abortAction;

MetaDataWriter::MetaDataWriter(QObject * parent)
: QObject(parent) {
	
}





/**
 * PUBLIC SLOTS
 **/

void MetaDataWriter::write() {
	QMap<QString, MetaData>::const_iterator i = _items.constBegin();

	// Signal started() should be emitted before maximumChanged()!
	emit started();
	emit maximumChanged(_items.count());

	int prg = 0;
	
	while (i != _items.constEnd()) {
		if (abortAction)
			return;
		
		const QString path = i.key();
		const MetaData metaData = i.value();

		TagLib::FileRef fileref(path.toUtf8().constData());

		int type = fileType(path);
		
		switch (type) {

			case FlacFile: {
				TagLib::FLAC::File * file = dynamic_cast<TagLib::FLAC::File*>(fileref.file());

				if (!file)
					break;

				writeTag(file->xiphComment(true), metaData);

				if (QSettings().value("FlacId3v2").toBool())
					writeTag(file->ID3v2Tag(true), metaData);

				file->removePictures();

				for (int i = 0; i < metaData.imageCount(); i++) {
					const MetaDataImage image = metaData.image(i);
					QByteArray * bytes = imageToBytes(image.image());

					TagLib::FLAC::Picture * flacPicture = new TagLib::FLAC::Picture();

					flacPicture->setType((TagLib::FLAC::Picture::Type)image.type());
					flacPicture->setData(TagLib::ByteVector(bytes->data(), bytes->size()));
					flacPicture->setDescription(TagLib::String(image.description().toStdString()));

					file->addPicture(flacPicture);

					delete bytes;
				}

				break;
			}

			case OggVorbisFile: {
				TagLib::Vorbis::File * file = dynamic_cast<TagLib::Vorbis::File*>(fileref.file());

				if (!file)
					break;

				writeTag(file->tag(), metaData);

				break;
			}
				
			case MpegFile: {
				TagLib::MPEG::File * file = dynamic_cast<TagLib::MPEG::File*>(fileref.file());

				if (!file)
					break;
				
				if (QSettings().value("MpegId3v1").toBool())
					writeGeneric(file->ID3v1Tag(true), metaData);
				else
					file->strip(TagLib::MPEG::File::ID3v1);

				writeTag(file->ID3v2Tag(true), metaData);

				break;
			}
			
			default:
				;
		}

		fileref.save();

		// File should be renamed
		if (metaData.has(MetaData::PathField)) {
			QFileInfo newFile(metaData.get(MetaData::PathField).toString());

			if (!newFile.dir().exists())
				newFile.dir().mkpath(".");
			
			QFile(path).rename(newFile.absoluteFilePath());

			// Check that deleting empty directories is allowed
			if (!QSettings().value("DeleteOrphans").toBool())
				continue;
			
			QDir oldDir = QFileInfo(path).dir();
			
			// Remove directories that are left empty
			while (oldDir.entryList(QStringList(), QDir::AllEntries | QDir::NoDotAndDotDot).isEmpty()) {
				const QString dirName = oldDir.dirName();
				
				oldDir.cdUp();
				oldDir.rmdir(dirName);
			}
		}

		i++;
		prg++;
		
		emit progress(prg);
	}

	emit finished();
}





/**
 * PRIVATE
 **/

int MetaDataWriter::fileType(const QString & path) const {
	const QString s = path.section('.', -1).toLower();

	if (s == "mp3" || s == "mpeg")
		return MpegFile;

	if (s == "ogg")
		return OggVorbisFile;

	if (s == "flac")
		return FlacFile;

	return UnknownFile;
}

void MetaDataWriter::writeGeneric(TagLib::Tag * tag, const MetaData & metaData) const {
	if (!tag || metaData.null())
		return;

	bool id3_latin = false;

	if (dynamic_cast<TagLib::ID3v2::Tag*>(tag)) {
		// 0 = UTF-8, 1 = ISO-8859-15
		if (QSettings().value("Id3v2Encoding").toInt() == 1)
			id3_latin = true;
	}

	if (dynamic_cast<TagLib::ID3v1::Tag*>(tag))
		id3_latin = true;

	foreach (MetaData::Field f, metaData.fields()) {
		TagLib::String str(metaData.get(f).toString().toUtf8().constData(), TagLib::String::UTF8);

		if (id3_latin)
			str = TagLib::String(metaData.get(f).toString().toLatin1().constData());
		
		switch (f) {
			case MetaData::ArtistField: tag->setArtist(str); break;
			case MetaData::AlbumField: tag->setAlbum(str); break;
			case MetaData::TitleField: tag->setTitle(str); break;
			case MetaData::CommentField: tag->setComment(str); break;
			case MetaData::GenreField: tag->setGenre(str); break;

			case MetaData::YearField: tag->setYear(str.toInt()); break;
			case MetaData::NumberField: tag->setTrack(str.toInt()); break;

			default: break;
		}
	}
}

void MetaDataWriter::writeTag(TagLib::ID3v2::Tag * tag, const MetaData & metaData) const {
	if (!tag || metaData.null())
		return;

	/**
	 * REMEMBER TO ADD MISSING FIELDS!
	 **/
	
	writeGeneric(tag, metaData);

	if (metaData.has(MetaData::UrlField)) {
		tag->removeFrames("WXXX");

		TagLib::String url = metaData.get(MetaData::UrlField).toString().toStdString();
		
		if (!url.isEmpty()) {
			TagLib::ID3v2::UserUrlLinkFrame * frame = new TagLib::ID3v2::UserUrlLinkFrame();
			frame->setText(url);

			tag->removeFrames("WXXX");
			tag->addFrame(frame);
		}
	}

	if (metaData.has(MetaData::NumberField)) {
		TagLib::String num = metaData.get(MetaData::NumberField).toString().toStdString();

		if (metaData.has(MetaData::MaxNumberField)) {
			TagLib::String mnum = metaData.get(MetaData::MaxNumberField).toString().toStdString();

			if (!mnum.isEmpty() && mnum.toInt() > 0)
				num += "/" + mnum;
		}

		TagLib::ID3v2::TextIdentificationFrame * frame =
			new TagLib::ID3v2::TextIdentificationFrame("TRCK");

		frame->setText(num);

		tag->removeFrames("TRCK");
		tag->addFrame(frame);
	}

	if (metaData.has(MetaData::DiscNumberField)) {		
		TagLib::String disc = metaData.get(MetaData::DiscNumberField).toString().toStdString();

		TagLib::ID3v2::TextIdentificationFrame * frame =
			new TagLib::ID3v2::TextIdentificationFrame("TPOS");

			frame->setText(metaData.get(MetaData::DiscNumberField).toString().toStdString());

		tag->removeFrames("TPOS");
		tag->addFrame(frame);
	}

	tag->removeFrames("APIC");

	for (int i = 0; i < metaData.imageCount(); i++) {
		const MetaDataImage image = metaData.image(i);

		QByteArray * bytes = imageToBytes(image.image());
		
		TagLib::ID3v2::AttachedPictureFrame * frame = new TagLib::ID3v2::AttachedPictureFrame();

		frame->setMimeType("image/jpeg");
		frame->setType((TagLib::ID3v2::AttachedPictureFrame::Type)image.type());
		frame->setDescription(image.description().toStdString());
		
		frame->setPicture(TagLib::ByteVector(bytes->data(), bytes->size()));

		tag->addFrame(frame);

		delete bytes;
	}
	
}

void MetaDataWriter::writeTag(TagLib::Ogg::XiphComment * tag, const MetaData & metaData) const {
	if (!tag || metaData.null())
		return;
	
	writeGeneric(tag, metaData);

	QHash<MetaData::Field, TagLib::String> maps;

	// These mappings follow a pattern that seems to be quite popular albeit
	// not really defined in the Xiph specs.
	maps.insert(MetaData::ComposerField, "COMPOSER");
	maps.insert(MetaData::EncoderField, "ENCODED-BY");
	maps.insert(MetaData::DiscNumberField, "DISCNUMBER");
	maps.insert(MetaData::MaxNumberField, "TRACKTOTAL");
	maps.insert(MetaData::OriginalArtistField, "PERFORMER");
	maps.insert(MetaData::UrlField, "LICENSE");

	foreach (MetaData::Field f, metaData.fields()) {
		if (maps.contains(f)) {
			tag->addField(
				maps.value(f),
				TagLib::String(metaData.get(f).toString().toUtf8().constData(), TagLib::String::UTF8)
			);
		}
	}

	// From now on legacy covers will be stripped with no further questions.
	if (QSettings().value("ConvertVorbisLegacyCovers").toBool() && metaData.imageCount()) {
		tag->removeField("COVERART");
		tag->removeField("COVERARTDESCRIPTION");
		tag->removeField("COVERARTMIME");
		tag->removeField("COVERARTTYPE");
	}

	tag->removeField("METADATA_BLOCK_PICTURE");

	for (int i = 0; i < metaData.imageCount(); i++) {
		QByteArray data;
		QDataStream s(&data, QIODevice::WriteOnly);
		const MetaDataImage image = metaData.image(i);

		QByteArray * bytes = imageToBytes(image.image());
		qint32 ds = image.description().size();

		s << image.type() << 10;
		s.writeRawData("image/jpeg", 10);

		s << ds;
		s.writeRawData(image.description().toAscii().constData(), ds);

		s << (qint32)image.image().width() << (qint32)image.image().height();
		s << (qint32)image.image().depth() << (qint32)0 << bytes->size();

		data.append(*bytes);

		tag->addField("METADATA_BLOCK_PICTURE", data.toBase64().data(), false);

		delete bytes;
	}
	

}

QByteArray * MetaDataWriter::imageToBytes(const QImage & img) const {
	QByteArray * d = new QByteArray;
	QBuffer b(d);

	b.open(QIODevice::WriteOnly);

	QImageWriter w(&b, "JPEG");
	w.write(img);

	return d;
}