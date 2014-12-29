#ifndef COQUILLO_METADATA_FILEWRITER_H
#define COQUILLO_METADATA_FILEWRITER_H

#include <QObject>
#include <QRunnable>
#include "metadata.h"

namespace TagLib {
    class File;
}

namespace Coquillo {
    namespace MetaData {
        class MetaData;

        class FileWriter : public QObject, public QRunnable {
            Q_OBJECT
            public:
                FileWriter(const QList<MetaData> & files, QObject * parent = 0);
                void run();

            private:
                bool isFlacFile(const TagLib::File * file) const;
                bool isMpegFile(const TagLib::File * file) const;
                bool isVorbisFile(const TagLib::File * file) const;

                QList<MetaData> _files;
        };
    }
}

#endif
