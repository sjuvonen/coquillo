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

#ifndef METADATAREADER_H
#define METADATAREADER_H

#include <QImage>

#include "def_MetaData.h"

namespace TagLib {
	class Tag;

	namespace ID3v1 {
		class Tag;
	}

	namespace ID3v2 {
		class Frame;
		class Tag;
	}

	namespace Ogg {
		class XiphComment;
	}
}

class QString;

class MetaDataImage;

class MetaDataReader {
	public:
		static MetaData getTags(const QString & path);

		/**
		 * Returns a list of pointers to images.
		 * MetaDataReader doesn't manage the pointers in any way so the data
		 * must be deleted manually.
		 **/
		static QList<MetaDataImage> getImages(const QString & path);

	private:
		static QString fileType(const QString & path);

		static void readFLACFile(MetaData *);
		static void readMPEGFile(MetaData *);
		static void readVorbisFile(MetaData *);

		static bool readGenericTag(TagLib::Tag * tag, MetaData * metaData);

		static bool readID3v1Tag(TagLib::ID3v1::Tag * tag, MetaData * metaData);
		static bool readID3v2Tag(TagLib::ID3v2::Tag * tag, MetaData * metaData);
		static bool readXiphTag(TagLib::Ogg::XiphComment * tag, MetaData * metaData);

		static QList<MetaDataImage> readLegacyXiphPictures(TagLib::Ogg::XiphComment * tag);
		static QList<MetaDataImage> readXiphPictures(TagLib::Ogg::XiphComment * tag);
};

#endif
