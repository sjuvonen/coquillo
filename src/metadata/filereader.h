#ifndef COQUILLO_METADATA_FILEREADER_H
#define COQUILLO_METADATA_FILEREADER_H

#include <QObject>

namespace TagLib {
    class File;
}

namespace Coquillo {
    namespace MetaData {
        class MetaData;

        class FileReader : public QObject {
            Q_OBJECT

            public:
                static FileReader * create(const QString & file);

                FileReader(const QString & file, QObject * parent = 0);
                virtual ~FileReader() { }

                inline QString path() const { return _file; }
                virtual void read() = 0;

            public slots:
                void start() { read(); }

            signals:
                void finished(const MetaData & metaData);

            protected:
                void finish(const TagLib::File & file, MetaData & metaData);

            private:
                enum FileType { MpegFile, OggFile, FlacFile };
                QString _file;
        };
    }
}

#endif
