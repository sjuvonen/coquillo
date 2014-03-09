#ifndef COQUILLO_METADATA_FILETYPES_H
#define COQUILLO_METADATA_FILETYPES_H

#include "filereader.h"

namespace Coquillo {
    namespace MetaData {
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
}

#endif
