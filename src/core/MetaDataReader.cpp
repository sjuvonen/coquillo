
#include <taglib/taglib.h>
#include <taglib/fileref.h>

#include <taglib/attachedpictureframe.h>
#include <taglib/flacfile.h>
#include <taglib/mp4tag.h>
#include <taglib/mpegfile.h>
#include <taglib/vorbisfile.h>
#include <taglib/id3v1tag.h>
#include <taglib/id3v2tag.h>
#include <taglib/oggfile.h>
#include <taglib/xiphcomment.h>
#include <taglib/xingheader.h>

#include <QDataStream>
#include <QFileInfo>
#include <QTextCodec>
#include <QSettings>
#include <QString>
#include <QStringList>

//#include "ImageCache.h"
#include "MetaData.h"
#include "MetaDataImage.h"
#include "MetaDataReader.h"

#include "globals.h"

#include <QDebug>

#define T2QString(s, toUtf) toUtf ? QString::fromUtf8(s.toCString(true)) : QString::fromLatin1(s.toCString(false))

MetaData MetaDataReader::getTags(const QString & path) {
	if (path.isEmpty() || !QFileInfo(path).exists())
		return MetaData();

	TagLib::FileRef file(path.toUtf8().constData(), true, TagLib::AudioProperties::Accurate);

	if (!file.tag())
		return MetaData();

	MetaData item;
	item.set(MetaData::PathField, path);

	if (file.audioProperties())
		item.set(MetaData::LengthField, file.audioProperties()->length());

	const QString type = fileType(path);

	if (type == "mpeg") readMPEGFile(&item);
	else if (type == "flac") readFLACFile(&item);
	else if (type == "ogg") readVorbisFile(&item);
	else readGenericTag(file.tag(), &item);

	// This adds missing fields. It's important to do this so that MetaDataModel would accurately
	// identify modified fields.
	for (int i = 0; i < MetaData::PicturesField; i++) {
		MetaData::Field f = static_cast<MetaData::Field>(i);

		if (!item.has(f))
			item.set(f, QVariant(""));
	}

	return item;
}

QList<MetaDataImage> MetaDataReader::readLegacyXiphPictures(TagLib::Ogg::XiphComment * tag) {
	QList<MetaDataImage> pics;

	if (QSettings().value("ConvertVorbisLegacyCovers").toBool()) {
		if (tag->contains("COVERART")) {
			TagLib::Ogg::FieldListMap fields = tag->fieldListMap();
			TagLib::StringList data, descr, mime, type;

			data = fields["COVERART"];

			if (fields.contains("COVERARTDESCRIPTION"))
				descr = fields["COVERARTDESCRIPTION"];

			if (fields.contains("COVERARTMIME"))
				mime = fields["COVERARTMIME"];

			for (uint i = 0; i < data.size(); i++) {
				TagLib::String d = data[i];
				const QByteArray raw = QByteArray::fromRawData((char*)(d.toCString()), d.size());
				const QImage img = QImage::fromData( QByteArray::fromBase64(raw) );
				const QString dsc = descr.size() > i ? descr[i].toCString(true) : QString();

				pics << MetaDataImage(img, 0, dsc);
			}
		}
	}

	return pics;
}

QList<MetaDataImage> MetaDataReader::readXiphPictures(TagLib::Ogg::XiphComment * tag) {
	QList<MetaDataImage> pics;

	if (tag->fieldListMap().contains("METADATA_BLOCK_PICTURE")) {
		TagLib::StringList blocks = tag->fieldListMap()["METADATA_BLOCK_PICTURE"];

		for (uint i = 0; i < blocks.size(); i++) {
			QByteArray data = QByteArray::fromBase64( blocks[i].toCString() );
			QDataStream s(&data, QIODevice::ReadOnly);

			int type;
			uint mimelen;
			int descrlen;
			int datalen;

			int w;
			int h;
			int c;
			int ic;

			char * mime;
			char * descr;
			char * pic;

			s >> type;
			s >> mimelen;

			mime = new char[mimelen+1];
			s.readRawData(mime, mimelen);

			mime[mimelen] = 0;

			s >> descrlen;

			descr = new char[descrlen+1];
			s.readRawData(descr, descrlen);

			descr[descrlen] = 0;

			s >> w >> h >> c >> ic >> datalen;

			if (!datalen) {
				qDebug() << "MetaDataReader(Xiph): Zero data length -> invalid picture!";
				continue;
			}

			pic = new char[datalen];
			s.readRawData(pic, datalen);

			QString mimeType = QString::fromUtf8(mime, mimelen);
			QString description = QString::fromUtf8(descr, descrlen);
			QImage picture = QImage::fromData( QByteArray(pic, datalen) );

			if (!picture.isNull()) {
				pics << MetaDataImage(picture, type, description);
			}
		}

	}

	return pics;
}


QString MetaDataReader::fileType(const QString & path) {
	const QString s = path.section('.', -1).toLower();

	if (s == "mp3")
		return "mpeg";

	return s;
}

void MetaDataReader::readFLACFile(MetaData * metaData) {
	TagLib::FLAC::File file( metaData->get(MetaData::PathField).toString().toUtf8().constData() );

	readGenericTag(file.xiphComment(), metaData);
	readXiphTag(file.xiphComment(), metaData);

	if (Coquillo::Flac::id3v2)
		readID3v2Tag(file.ID3v2Tag(), metaData);

	#if TAGLIB_MINOR_VERSION >= 7 || TAGLIB_MAJOR_VERSION > 1

	TagLib::List<TagLib::FLAC::Picture*> list = file.pictureList();

	bool toUtf = true;

	for (uint i = 0; i < list.size(); i++) {
		const TagLib::ByteVector data = list[i]->data();
		const QImage img = QImage::fromData( (const uchar*)data.data(), data.size() );
		const QString descr = T2QString(list[i]->description(), toUtf);

		// The FLAC picture types pretty much map to the types of ID3V2.
		int type = list[i]->type();

		metaData->addImage(MetaDataImage(img, type, descr));
	}

	#endif
}

void MetaDataReader::readMPEGFile(MetaData * metaData) {
	TagLib::MPEG::File file( metaData->get(MetaData::PathField).toString().toUtf8().constData() );


	if (file.ID3v2Tag()) {
		readID3v2Tag(file.ID3v2Tag(), metaData);

		TagLib::ID3v2::FrameListMap map = file.ID3v2Tag()->frameListMap();

		if (map.contains("APIC")) {
			TagLib::ID3v2::FrameList list = map["APIC"];

			for (uint i = 0; i < list.size(); i++) {
				TagLib::ID3v2::AttachedPictureFrame * frame = static_cast<TagLib::ID3v2::AttachedPictureFrame*>(list[i]);

				const QImage img = QImage::fromData((uchar*)frame->picture().data(), frame->picture().size());
				const QString descr = frame->description().toCString(true);
				int type = frame->type();

				metaData->addImage(MetaDataImage(img, type, descr));
			}
		}
	}


	if (Coquillo::Mpeg::id3v1)
		readID3v1Tag(file.ID3v1Tag(), metaData);
}

void MetaDataReader::readVorbisFile(MetaData * metaData) {
	TagLib::Ogg::Vorbis::File file( metaData->get(MetaData::PathField).toString().toUtf8().constData() );
	readXiphTag(file.tag(), metaData);

	metaData->setImages(readXiphPictures(file.tag()));
}





bool MetaDataReader::readGenericTag(TagLib::Tag * tag, MetaData * metaData) {
	if (!tag || !metaData)
		return false;

	bool toUtf = Coquillo::encoding == Coquillo::UTF_8;

	const QString title = T2QString(tag->title(), toUtf);
	const QString artist = T2QString(tag->artist(), toUtf);
	const QString album = T2QString(tag->album(), toUtf);
	const QString genre = T2QString(tag->genre(), toUtf);
	const QString comment = T2QString(tag->comment(), toUtf);

	// Currently data from all tags is mixed up, so we want to always use the first data available.
	// Therefore it's also important to read tags in prioritized order.
	// Empty fields are added because otherwise the editor widget does not work properly.
	// (If model returns a null QVariant, QDataWidgetMapper will use the previous track's value
	// for that specific field.)

	if (!metaData->has(MetaData::TitleField))
		metaData->set(MetaData::TitleField, title);

	if (!metaData->has(MetaData::ArtistField))
		metaData->set(MetaData::ArtistField, artist);

	if (!metaData->has(MetaData::AlbumField))
		metaData->set(MetaData::AlbumField, album);

	if (!metaData->has(MetaData::GenreField))
		metaData->set(MetaData::GenreField,  genre);

	if (!metaData->has(MetaData::CommentField))
		metaData->set(MetaData::CommentField, comment);

	if (!metaData->has(MetaData::YearField))
		metaData->set(MetaData::YearField, tag->year());

	if (!metaData->has(MetaData::NumberField))
		metaData->set(MetaData::NumberField, tag->track());

	if (!metaData->has(MetaData::MaxNumberField))
		metaData->set(MetaData::MaxNumberField, 0);

	if (!metaData->has(MetaData::DiscNumberField))
		metaData->set(MetaData::DiscNumberField, 0);

	return true;
}

bool MetaDataReader::readID3v1Tag(TagLib::ID3v1::Tag * tag, MetaData * metaData) {
	if (!tag || !metaData)
		return false;

	return readGenericTag(tag, metaData);
}

#define cstr(field, unicode) m[field].front()->toString().toCString(unicode)
#define qtstr(field, unicode) toQString(cstr(field, unicode), Coquillo::encoding)
#define hasField(field) m.contains(field) && !m[field].isEmpty()

bool MetaDataReader::readID3v2Tag(TagLib::ID3v2::Tag * tag, MetaData * metaData) {
	if (!tag)
		return false;

	TagLib::ID3v2::FrameListMap m = tag->frameListMap();

	bool toUnicode = ( Coquillo::encoding == Coquillo::UTF_8 );

	if (hasField("TCOM")) {
		metaData->set(MetaData::ComposerField, qtstr("TCOM", toUnicode));
	}

	if (hasField("TENC")) {
		metaData->set(MetaData::EncoderField, qtstr("TENC", toUnicode));
	}

	if (hasField("TOPE")) {
		metaData->set(MetaData::OriginalArtistField, qtstr("TOPE", toUnicode));
	}

	if (hasField("TPE2")) {
        metaData->set(MetaData::AlbumArtistField, qtstr("TPE2", toUnicode));
    }

	if (hasField("TPOS")) {
		metaData->set(MetaData::DiscNumberField, qtstr("TPOS", toUnicode));
	}

	if (hasField("WXXX")) {
		// URL field actually contains (not always?) an array which is marked by "[] ".
		metaData->set(MetaData::UrlField, QString(qtstr("WXXX", toUnicode)).remove(QRegExp("^\\[\\] ")));
	}

	if (hasField("TRCK")) {
		const QString trck = qtstr("TRCK", toUnicode);
		const QStringList numbers = trck.split('/');

		metaData->set(MetaData::NumberField, numbers[0].toInt());

		if (numbers.count() >= 2)
			metaData->set(MetaData::MaxNumberField, numbers[1].toInt());
	}

	return readGenericTag(tag, metaData);
}

#undef hasField
#undef qtstr

#define qtstr(field, unicode) toQString( m[field].front().toCString(unicode), Coquillo::encoding )
#define hasField(field) m.contains(field) && !m[field].isEmpty()

bool MetaDataReader::readXiphTag(TagLib::Ogg::XiphComment * tag, MetaData * metaData) {
	if (!tag)
		return false;

	TagLib::Ogg::FieldListMap m = tag->fieldListMap();

	bool toUnicode = ( Coquillo::encoding == Coquillo::UTF_8 );

	if (tag->contains("COMPOSER"))
		metaData->set(MetaData::ComposerField, qtstr("COMPOSER", toUnicode));

	if (tag->contains("PERFORMER"))
		metaData->set(MetaData::OriginalArtistField, qtstr("PERFORMER", toUnicode));

	if (tag->contains("LICENSE"))
		metaData->set(MetaData::UrlField, qtstr("LICENSE", toUnicode));

	if (tag->contains("DISCNUMBER"))
		metaData->set(MetaData::DiscNumberField, m["DISCNUMBER"].front().toInt());

	if (tag->contains("TRACKNUMBER"))
		metaData->set(MetaData::NumberField, m["TRACKNUMBER"].front().toInt());

	if (tag->contains("TRACKTOTAL"))
		metaData->set(MetaData::MaxNumberField, m["TRACKTOTAL"].front().toInt());

	if (tag->contains("ENCODED-BY"))
		metaData->set(MetaData::EncoderField, qtstr("ENCODED-BY", toUnicode));

	if (tag->contains("DESCRIPTION"))
		metaData->set(MetaData::CommentField, qtstr("DESCRIPTION", toUnicode));

	return readGenericTag(tag, metaData);
}

#undef hasField
#undef qtstr
#undef cstr
