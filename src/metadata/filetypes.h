#ifndef COQUILLO_FILETYPES_H
#define COQUILLO_FILETYPES_H

#include "filereader.h"

namespace Coquillo {
    class MpegReader : public FileReader {
        public:
            MpegReader(const QString & file, QObject * parent = 0)
            : FileReader(file, parent) { }

            void read();
    };

    class OggVorbisReader : public FileReader {
        public:
            OggVorbisReader(const QString & file, QObject * parent = 0)
            : FileReader(file, parent) { }

            void read();
    };

    class FlacReader : public FileReader {
        public:
            FlacReader(const QString & file, QObject * parent = 0)
            : FileReader(file, parent) { }

            void read();
    };
}

#endif
