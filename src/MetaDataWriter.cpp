/***********************************************************************
* Copyright (c) 2011 Samu Juvonen <samu.juvonen@gmail.com>
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
************************************************************************/

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

#include "MetaDataImage.h"
#include "MetaDataWriter.h"

#include <map>
#include <string>

#include <QBuffer>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QImageWriter>
#include <QMap>

#include <QDebug>

#include "globals.h"

#define Q2TString(s, toUtf) toUtf ? s.toUtf8().constData() : s.toLatin1().constData()

typedef TagLib::ID3v2::AttachedPictureFrame APFrame;

MetaDataWriter::MetaDataWriter(QObject * parent) : QObject(parent) {

}

void MetaDataWriter::write() {
	while (!_metaData.isEmpty()) {
		MetaData metaData = _metaData.takeFirst();

		qDebug() << metaData;

		if (metaData.contains("_OldPath") && metaData["_OldPath"] != metaData["Path"]) {
			QFileInfo fi(metaData.value("Path").toString());
			QDir dir = fi.dir();

			if (!dir.exists())
				dir.mkpath(".");

			qDebug() << "Test dir:" << dir.exists();

			QFile file(metaData.value("_OldPath").toString());
			file.rename(metaData.value("Path").toString());
		}

		TagLib::FileRef file( metaData.value("Path").toString().toUtf8().constData() );

		int type = fileType(metaData.value("Path").toString());

		switch (type) {
			case MPEG:
				if (TagLib::MPEG::File * f = dynamic_cast<TagLib::MPEG::File*>(file.file())) {
					if (Coquillo::Mpeg::id3v1)
						writeID3v1(f->ID3v1Tag(true), metaData);
					else
						f->strip(TagLib::MPEG::File::ID3v1);

					writeID3v2(f->ID3v2Tag(true), metaData);
				}
				break;

			case FLAC:
				if (TagLib::FLAC::File * f = dynamic_cast<TagLib::FLAC::File*>(file.file())) {
					writeXiph(f->xiphComment(true), metaData);

					#if TAGLIB_MINOR_VERSION >= 7
					if (metaData.contains("Pictures")) {
						f->removePictures();

						QVariantList list = metaData.value("Pictures").toList();

						foreach (const QVariant p, list) {
							const MetaDataImage blob = p.value<MetaDataImage>();
							QByteArray * d = imageToBytes(blob.image());

							const TagLib::String descr(
								blob.description().toUtf8().constData(),
								TagLib::String::UTF8
							);

							TagLib::FLAC::Picture * p = new TagLib::FLAC::Picture;
							p->setType(static_cast<TagLib::FLAC::Picture::Type>(blob.type()));
							p->setData( TagLib::ByteVector(d->data(), d->size()) );
							p->setDescription(descr);

							f->addPicture(p);

							delete d;
						}
					}
					#endif


					if (Coquillo::Flac::id3v2)
						writeID3v2(f->ID3v2Tag(true), metaData);
				} else if (TagLib::Ogg::FLAC::File * f = dynamic_cast<TagLib::Ogg::FLAC::File*>(file.file()))
					writeXiph(f->tag(), metaData);
				break;

			case OggVorbis: {
				TagLib::Ogg::Vorbis::File * f = dynamic_cast<TagLib::Ogg::Vorbis::File*>(file.file());

				if (f)
					writeXiph(f->tag(), metaData);
				break;
			}

			default:
				writeGeneric(file.tag(), metaData);
		}

		file.save();

	}
}





int MetaDataWriter::fileType(const QString & path) const {
	const QString s = path.section('.', -1).toLower();

	if (s == "mp3" || s == "mpeg")
		return MPEG;

	if (s == "ogg")
		return OggVorbis;

	if (s == "flac")
		return FLAC;

	return UnknownMedia;
}

void MetaDataWriter::writeID3v1(TagLib::ID3v1::Tag * tag, const MetaData & metaData) {
	if (!tag)
		return;

	writeGeneric(tag, metaData);

	/*
	const QString artist = metaData.value("Artist").toString();
	const QString title = metaData.value("Title").toString();
	const QString album = metaData.value("Album").toString();
	const QString genre = metaData.value("Genre").toString();
	const QString comment = metaData.value("Comment").toString();
	int n = metaData.value("Number").toInt();
	int year = metaData.value("Year").toInt();

	tag->setArtist(artist.isEmpty() ? TagLib::String() : artist.toLatin1().constData());
	tag->setAlbum(album.isEmpty() ? TagLib::String() : album.toLatin1().constData());
	tag->setComment(comment.isEmpty() ? TagLib::String() : comment.toLatin1().constData());
	tag->setGenre(genre.isEmpty() ? TagLib::String() : genre.toLatin1().constData());
	tag->setTitle(title.isEmpty() ? TagLib::String() : title.toLatin1().constData());
	tag->setTrack(n);
	tag->setYear(year);
	*/
}

void MetaDataWriter::writeID3v2(TagLib::ID3v2::Tag * tag, const MetaData & metaData) {
	if (!tag || metaData.isEmpty())
		return;

	writeGeneric(tag, metaData);

	std::map<std::string, std::string> m;

	m["OriginalArtist"] = "TOPE";
	m["Composer"] = "TCOM";
	m["Encoder"] = "TENC";
	m["Disc"] = "TPOS";

	bool toUnicode = ( Coquillo::encoding == Coquillo::UTF_8 );

	for (std::map<std::string, std::string>::iterator i = m.begin(); i != m.end(); i++) {
		if (metaData.contains(i->first.c_str())) {
			tag->removeFrames(i->second.c_str());

			const QString s = metaData.value(i->first.c_str()).toString();

			if (!s.isEmpty()) {
				TagLib::ID3v2::TextIdentificationFrame * frame = new TagLib::ID3v2::TextIdentificationFrame(i->second.c_str());
				TagLib::String::Type type = toUnicode ? TagLib::String::UTF8 : TagLib::String::Latin1;

				frame->setText( TagLib::String( Q2TString(s, toUnicode), type) );
				tag->addFrame(frame);
			}
		}
	}

	if (metaData.contains("Url")) {
		tag->removeFrames("WXXX");

		const QString url = metaData.value("Url").toString();

		if (!url.isEmpty()) {
			TagLib::ID3v2::UserUrlLinkFrame * frame = new TagLib::ID3v2::UserUrlLinkFrame;

			if (toUnicode)
				frame->setText( url.toUtf8().constData() );
			else
				frame->setText( url.toLatin1().constData() );

			tag->addFrame(frame);
		}
	}

	if (metaData.contains("Pictures")) {
		QList<QVariant> pics = metaData.value("Pictures").toList();

		tag->removeFrames("APIC");

		foreach (const QVariant p, pics) {
			const MetaDataImage pic = p.value<MetaDataImage>();

			TagLib::ID3v2::AttachedPictureFrame * frame = new TagLib::ID3v2::AttachedPictureFrame;

			frame->setMimeType("image/jpeg");

			frame->setType(
				static_cast<TagLib::ID3v2::AttachedPictureFrame::Type>(pic.type())
			);

			if (toUnicode) {
				frame->setDescription(
					TagLib::String(
						pic.description().toUtf8().constData(),
						TagLib::String::UTF8
					)
				);
			} else {
				frame->setDescription(
					TagLib::String(
						pic.description().toLatin1().constData(),
						TagLib::String::Latin1
					)
				);
			}

			QByteArray * d = imageToBytes(pic.image());

			frame->setPicture(
				TagLib::ByteVector(
					d->data(),
					d->size()
				)
			);

			delete d;

			tag->addFrame(frame);
		}
	}

	/*
	 * Note: metaData is guaranteed to contain field 'Number' always when it
	 * contains 'MaxNumber' aswell.
	 */

	if (metaData.contains("Number")) {
		tag->removeFrames("TRCK");

		QString n = metaData.value("Number").toString();

		if (metaData.contains("MaxNumber") && metaData.value("MaxNumber").toInt() > 0)
			n.append(QString("/%1").arg(metaData.value("MaxNumber").toString()));

		if (!n.isEmpty()) {
			TagLib::ID3v2::TextIdentificationFrame * frame = new TagLib::ID3v2::TextIdentificationFrame("TRCK");
			frame->setText(n.toUtf8().constData());

			tag->addFrame(frame);
		}
	}
}

void MetaDataWriter::writeXiph(TagLib::Ogg::XiphComment * tag, const MetaData & metaData) {
	if (!tag || metaData.isEmpty())
		return;

	writeGeneric(tag, metaData);

	if (metaData.contains("Comment")) {
		const TagLib::String s(
			metaData.value("Comment").toString().toUtf8().constData(),
			TagLib::String::UTF8
		);

		if (!s.isEmpty())
			tag->setComment(s);
		else
			tag->removeField("DESCRIPTION");
	}

	if (metaData.contains("Composer")) {
		const TagLib::String s(
			metaData.value("Composer").toString().toUtf8().constData(),
			TagLib::String::UTF8
		);

		if (!s.isEmpty())
			tag->addField("COMPOSER", s);
		else
			tag->removeField("COMPOSER");
	}

	if (metaData.contains("Encoder")) {
		const TagLib::String s(
			metaData.value("Encoder").toString().toUtf8().constData(),
			TagLib::String::UTF8
		);

		if (!s.isEmpty())
			tag->addField("ENCODED-BY", s);
		else
			tag->removeField("ENCODED-BY");
	}

	if (metaData.contains("Disc")) {
		if (metaData.value("Disc").toInt() > 0)
			tag->addField(
				"DISCNUMBER",
				metaData.value("Disc").toString().toUtf8().constData()
			);
		else
			tag->removeField("DISCNUMBER");
	}

	if (metaData.contains("MaxNumber")) {
		if (metaData.value("MaxNumber").toInt() > 0) {
			tag->addField(
				"TRACKTOTAL",
				metaData.value("MaxNumber").toString().toUtf8().constData()
			);
	}else
			tag->removeField("TRACKTOTAL");
	}

	if (metaData.contains("OriginalArtist")) {
		const TagLib::String s(
			metaData.value("OriginalArtist").toString().toUtf8().constData(),
			TagLib::String::UTF8
		);

		if (!s.isEmpty())
			tag->addField("PERFORMER", s);
		else
			tag->removeField("PERFORMER");
	}

	if (metaData.contains("Url")) {
		const TagLib::String s(
			metaData.value("Url").toString().toUtf8().constData(),
			TagLib::String::UTF8
		);

		if (!s.isEmpty())
			tag->addField("LICENSE", s);
		else
			tag->removeField("LICENSE");
	}

	if (Coquillo::OggVorbis::stripLegacyCovers) {
		tag->removeField("COVERART");
		tag->removeField("COVERARTDESCRIPTION");
		tag->removeField("COVERARTMIME");
		tag->removeField("COVERARTTYPE");
	}

	if (metaData.contains("Pictures"))
		writeXiphPictures(tag, metaData.value("Pictures").toList());
}

void MetaDataWriter::writeGeneric(TagLib::Tag * tag, const MetaData & metaData) {
	if (!tag || metaData.isEmpty())
		return;

	if (metaData.contains("Artist")) {
		const TagLib::String s(
			metaData.value("Artist").toString().toUtf8().constData(),
			TagLib::String::UTF8
		);
		tag->setArtist(s);
	}

	if (metaData.contains("Album")) {
		const TagLib::String s(
			metaData.value("Album").toString().toUtf8().constData(),
			TagLib::String::UTF8
		);
		tag->setAlbum(s);
	}

	if (metaData.contains("Title")) {
		const TagLib::String s(
			metaData.value("Title").toString().toUtf8().constData(),
			TagLib::String::UTF8
		);
		tag->setTitle(s);
	}

	if (metaData.contains("Comment")) {
		const QString comment = metaData.value("Comment").toString();

		if (comment.isEmpty())
			tag->setComment( TagLib::String() );
		else {
			tag->setComment(
				TagLib::String(
					metaData.value("Comment").toString().toUtf8().constData(),
					TagLib::String::UTF8
				)
			);
		}
	}

	if (metaData.contains("Genre")) {
		const TagLib::String s(
			metaData.value("Genre").toString().toUtf8().constData(),
			TagLib::String::UTF8
		);
		tag->setGenre(s);
	}

	if (metaData.contains("Year"))
		tag->setYear( metaData.value("Year").toInt() );

	if (metaData.contains("Number"))
		tag->setTrack( metaData.value("Number").toInt() );
}

QByteArray * MetaDataWriter::imageToBytes(const QImage & img) const {
	QByteArray * d = new QByteArray;
	QBuffer b(d);

	b.open(QIODevice::WriteOnly);

	QImageWriter w(&b, "JPEG");
	w.write(img);

	return d;
}

void MetaDataWriter::writeXiphPictures(TagLib::Ogg::XiphComment * tag, const QVariantList & pics) {
	if (!tag)
		return;

	tag->removeField("METADATA_BLOCK_PICTURE");

	foreach (const QVariant variant, pics) {
		QByteArray data;
		QDataStream s(&data, QIODevice::WriteOnly);
		MetaDataImage pic = variant.value<MetaDataImage>();

		QByteArray * picdata = imageToBytes(pic.image());
		qint32 ds = pic.description().size();

		s << pic.type() << 10;
		s.writeRawData("image/jpeg", 10);

		s << ds;
		s.writeRawData(pic.description().toAscii().constData(), ds);

		s << (qint32)pic.width() << (qint32)pic.height();
		s << (qint32)pic.image().depth() << (qint32)0 << picdata->size();

		data.append(*picdata);

		tag->addField("METADATA_BLOCK_PICTURE", data.toBase64().data(), false);

		delete picdata;
	}
}
