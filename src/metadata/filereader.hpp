#ifndef COQUILLO_METADATA_FILEREADER_H
#define COQUILLO_METADATA_FILEREADER_H

#include <QObject>
#include <QRunnable>
#include <QStringList>

namespace TagLib {
    class File;
}

namespace Coquillo {
    namespace MetaData {
        class MetaData;

        class FileReader : public QObject, public QRunnable {
            Q_OBJECT

            public:
                FileReader(const QStringList & files, QObject * parent = 0);
                void run();

            signals:
                void resolved(const MetaData & metaData);

            private:
                bool isFlacFile(const TagLib::File * file) const;
                bool isMpegFile(const TagLib::File * file) const;
                bool isVorbisFile(const TagLib::File * file) const;

                enum FileType { MpegFile, OggFile, FlacFile };
                QStringList _files;
        };
    }
}

#endif
