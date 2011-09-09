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

#ifndef METADATAWRITER_H
#define METADATAWRITER_H

#include <QObject>

#include "def_MetaData.h"

namespace TagLib {
	class File;
	class Tag;

	namespace ID3v1 {
		class Tag;
	}

	namespace ID3v2 {
		class Tag;
	}

	namespace Ogg {
		class XiphComment;
	}
}

class MetaDataWriter : public QObject {
	Q_OBJECT

	public:
		enum MediaType { UnknownMedia, MPEG, FLAC, OggFLAC, OggVorbis };
		MetaDataWriter(QObject * parent=0);

		void clearQueue() { _metaData.clear(); }
		void queue(const MetaData & item) { _metaData << item; }
		void queue(const QList<MetaData> & items) { _metaData << items; }

	public slots:
		void write();

	private:
		QByteArray * imageToBytes(const QImage &) const;

		int fileType(const QString & path) const;

		void writeGeneric(TagLib::Tag * tag, const MetaData & metaData);
		void writeID3v1(TagLib::ID3v1::Tag * tag, const MetaData & metaData);
		void writeID3v2(TagLib::ID3v2::Tag * tag, const MetaData & metaData);
		void writeXiph(TagLib::Ogg::XiphComment * tag, const MetaData & metaData);

		void writeXiphPictures(TagLib::Ogg::XiphComment * tag, const QVariantList & pics);

		QList<MetaData> _metaData;
};

#endif
