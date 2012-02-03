#ifndef METADATAREADER_H
#define METADATAREADER_H

#include <QImage>

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
