#ifndef METADATAWRITER_H
#define METADATAWRITER_H

#include <QObject>

#include "globals.h"

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
