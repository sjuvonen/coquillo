#ifndef COQUILLO_TAGWRITER_TAGWRITERJOB_H
#define COQUILLO_TAGWRITER_TAGWRITERJOB_H

#include <QObject>
#include <QRunnable>
#include "tagcontainer.hpp"

namespace TagLib {
    class File;
}

namespace Coquillo {
    namespace Tags {
        class WriterJob : public QObject, public QRunnable {
            Q_OBJECT

            public:
                WriterJob(const Container & item);
                void run();

            signals:
                void finished();

            private:
                bool isFlacFile(const TagLib::File * file) const;
                bool isMpegFile(const TagLib::File * file) const;
                bool isVorbisFile(const TagLib::File * file) const;

                Container item;
        };
    }
}

#endif
