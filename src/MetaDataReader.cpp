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

#include <QFileInfo>
#include <QSettings>
#include <QString>
#include <QStringList>

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

#include "MetaDataImage.h"
#include "MetaDataReader.h"

#include <QDataStream>
#include <QTextCodec>
#include <QDebug>

#include "ImageCache.h"

#include "globals.h"

#define T2QString(s, toUtf) toUtf ? QString::fromUtf8(s.toCString(true)) : QString::fromLatin1(s.toCString(false))

MetaData MetaDataReader::getTags(const QString & path) {
	if (path.isEmpty() || !QFileInfo(path).exists())
		return MetaData();

	TagLib::FileRef file(path.toUtf8().constData(), true, TagLib::AudioProperties::Accurate);

	if (!file.tag())
		return MetaData();

	MetaData item;
	item.insert("Path", path);

	if (file.audioProperties())
		item.insert("Length", file.audioProperties()->length());

	const QString type = fileType(path);

	if (type == "mpeg") readMPEGFile(&item);
	else if (type == "flac") readFLACFile(&item);
	else if (type == "ogg") readVorbisFile(&item);
	else readGenericTag(file.tag(), &item);

	// This adds missing fields. It's important to do this so that MetaDataModel would accurately
	// identify modified fields.
	foreach (const QString field, g_fieldNames.values()) {
		if (!item.contains(field))
			item.insert(field, QString(""));
	}

	return item;
}

QList<MetaDataImage> MetaDataReader::getImages(const QString & path) {
	const QString type = fileType(path);
	QList<MetaDataImage> images;

	if (path.isEmpty())
		return images;

	if (type == "mpeg") {
		TagLib::MPEG::File file(path.toUtf8().constData());

		if (!file.ID3v2Tag())
			return images;

		TagLib::ID3v2::FrameListMap map = file.ID3v2Tag()->frameListMap();

		if (map.contains("APIC") && !map["APIC"].isEmpty()) {
			TagLib::ID3v2::FrameList list = map["APIC"];

			for (uint i = 0; i < list.size(); i++) {
				TagLib::ID3v2::AttachedPictureFrame * frame = static_cast<TagLib::ID3v2::AttachedPictureFrame*>(list[i]);
				const QImage img = QImage::fromData((uchar*)frame->picture().data(), frame->picture().size());
				const QString descr = frame->description().toCString(true);
				int type = frame->type();

				images << MetaDataImage(img, type, descr);
			}
		}
	} else if (type == "ogg") {
		TagLib::Ogg::Vorbis::File file( path.toUtf8().constData() );

		if (file.tag()) {
			images << readLegacyXiphPictures(file.tag());
			images << readXiphPictures(file.tag());
		}
	} else if (type == "flac") {

		#if TAGLIB_MINOR_VERSION >= 7 || TAGLIB_MAJOR_VERSION > 1

		TagLib::FLAC::File file( path.toUtf8().constData() );
		TagLib::List<TagLib::FLAC::Picture*> list = file.pictureList();

		bool toUtf = Coquillo::encoding == Coquillo::UTF_8;

		for (uint i = 0; i < list.size(); i++) {
			const TagLib::ByteVector data = list[i]->data();
			const QImage img = QImage::fromData( (const uchar*)data.data(), data.size() );
			const QString descr = T2QString(list[i]->description(), toUtf);

			// The FLAC picture types pretty much map to the types of ID3V2.
			int type = list[i]->type();

			images << MetaDataImage(img, type, descr);
		}

		#endif
	}

	return images;
}

QList<MetaDataImage> MetaDataReader::readLegacyXiphPictures(TagLib::Ogg::XiphComment * tag) {
	QList<MetaDataImage> pics;

	if (tag->contains("COVERART")) {
		TagLib::Ogg::FieldListMap fields = tag->fieldListMap();
		TagLib::StringList data, descr, mime, type;

		data = fields["COVERART"];

		if (fields.contains("COVERARTDESCRIPTION"))
			descr = fields["COVERARTDESCRIPTION"];

		if (fields.contains("COVERARTMIME"))
			mime = fields["COVERARTMIME"];

		if (fields.contains("COVERARTTYPE"))
			type = fields["COVERARTTYPE"];

		for (uint i = 0; i < data.size(); i++) {
			TagLib::String d = data[i];
			const QByteArray raw = QByteArray::fromRawData((char*)(d.toCString()), d.size());
			const QImage img = QImage::fromData( QByteArray::fromBase64(raw) );
			const QString dsc = descr.size() > i ? descr[i].toCString(true) : QString();
			const QString t = QString();

			pics << MetaDataImage(img, t, dsc);
		}
	}

	return pics;
}

QList<MetaDataImage> MetaDataReader::readXiphPictures(TagLib::Ogg::XiphComment * tag) {
	QList<MetaDataImage> pics;

	if (tag->contains("METADATA_BLOCK_PICTURE")) {
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

			if (!picture.isNull())
				pics << MetaDataImage(picture, type, description);
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
	TagLib::FLAC::File file( metaData->value("Path").toString().toUtf8().constData() );

	readGenericTag(file.xiphComment(), metaData);
	readXiphTag(file.xiphComment(), metaData);

	if (Coquillo::Flac::id3v2)
		readID3v2Tag(file.ID3v2Tag(), metaData);
}

void MetaDataReader::readMPEGFile(MetaData * metaData) {
	TagLib::MPEG::File file( metaData->value("Path").toString().toUtf8().constData() );

	readID3v2Tag(file.ID3v2Tag(), metaData);

	if (Coquillo::Mpeg::id3v1)
		readID3v1Tag(file.ID3v1Tag(), metaData);
}

void MetaDataReader::readVorbisFile(MetaData * metaData) {
	TagLib::Ogg::Vorbis::File file( metaData->value("Path").toString().toUtf8().constData() );
	readXiphTag(file.tag(), metaData);
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

	// We don't want to add empty tags nor overwrite existing values.
	// Currently data from all tags is mixed up, so we want to always use the first data available.
	// Therefore it's also important to read tags in prioritized order.

	if (!metaData->contains("Title") && !title.isEmpty())
		metaData->insert("Title", title);

	if (!metaData->contains("Artist") && !artist.isEmpty())
		metaData->insert("Artist", artist);

	if (!metaData->contains("Album") && !album.isEmpty())
		metaData->insert("Album", album);

	if (!metaData->contains("Genre") && !genre.isEmpty())
		metaData->insert("Genre",  genre);

	if (!metaData->contains("Comment") && !comment.isEmpty())
		metaData->insert("Comment", comment);

	if (!metaData->contains("Year") && tag->year() != 0)
		metaData->insert("Year", tag->year());

	if (!metaData->contains("Number") && tag->track() != 0)
		metaData->insert("Number", tag->track());

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

	if (hasField("TCOM"))
		metaData->insert( "Composer", qtstr("TCOM", toUnicode) );

	if (hasField("TENC"))
		metaData->insert("Encoder", qtstr("TENC", toUnicode));

	if (hasField("TOPE"))
		metaData->insert("OriginalArtist", qtstr("TOPE", toUnicode));

	if (hasField("TPOS"))
		metaData->insert("Disc", qtstr("TPOS", toUnicode));

	if (hasField("WXXX"))
		metaData->insert("Url", qtstr("WXXX", toUnicode));

	if (hasField("TRCK")) {
		const QString trck = qtstr("TRCK", toUnicode);
		const QStringList numbers = trck.split('/');

		qDebug() << trck;
		qDebug() << numbers;
		qDebug();

		metaData->insert("Number", numbers[0].toInt());

		if (numbers.count() >= 2)
			metaData->insert("MaxNumber", numbers[1].toInt());
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
		metaData->insert("Composer", qtstr("COMPOSER", toUnicode));

	if (tag->contains("PERFORMER"))
		metaData->insert("OriginalArtist", qtstr("PERFORMER", toUnicode));

	if (tag->contains("LICENSE"))
		metaData->insert("Url", qtstr("LICENSE", toUnicode));

	if (tag->contains("DISCNUMBER"))
		metaData->insert("Disc", m["DISCNUMBER"].front().toInt());

	if (tag->contains("TRACKNUMBER"))
		metaData->insert("Number", m["TRACKNUMBER"].front().toInt());

	if (tag->contains("TRACKTOTAL"))
		metaData->insert("MaxNumber", m["TRACKTOTAL"].front().toInt());

	if (tag->contains("ENCODED-BY"))
		metaData->insert("Encoder", qtstr("ENCODED-BY", toUnicode));

	if (tag->contains("DESCRIPTION"))
		metaData->insert("Comment", qtstr("DESCRIPTION", toUnicode));

	return readGenericTag(tag, metaData);
}

#undef hasField
#undef qtstr
#undef cstr
