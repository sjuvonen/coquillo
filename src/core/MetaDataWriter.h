#ifndef METADATAWRITER_H
#define METADATAWRITER_H

#include <QObject>

#include "MetaData.h"

namespace TagLib {
	class Tag;

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
		MetaDataWriter(QObject * parent=0);

		void enqueue(const QString & path, const MetaData & data) { _items.insert(path, data); }
		void setQueue(const QMap<QString, MetaData> & items) { _items = items; }

	signals:
		void progress(int);
		void maximumChanged(int);
		void started();
		void finished();

	public slots:
		void write();

	private:
		enum FileType { UnknownFile, MpegFile, FlacFile, OggVorbisFile };

		int fileType(const QString & path) const;

		void writeGeneric(TagLib::Tag * tag, const MetaData & metaData) const;
		
		void writeTag(TagLib::ID3v2::Tag * tag, const MetaData & metaData) const;
		void writeTag(TagLib::Ogg::XiphComment * tag, const MetaData & metaData) const;

		QByteArray * imageToBytes(const QImage & image) const;

		QMap<QString, MetaData> _items;
	
};

#endif