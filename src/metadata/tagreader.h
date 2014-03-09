#ifndef COQUILLO_TAGREADER_H
#define COQUILLO_TAGREADER_H

#include "metadata.h"

namespace TagLib {
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

namespace Coquillo {
    namespace MetaData {
        class TagReader {
            public:
                TagReader(TagLib::Tag * tag = 0);
                virtual ~TagReader() { }
                virtual Tag read() = 0;
                inline void setTag(TagLib::Tag * tag) { _tag = tag; }
                inline TagLib::Tag * tag() const { return _tag; }

            protected:
                Tag readCommon() const;

            private:
                TagLib::Tag * _tag;
        };

        class Id3v1Reader : public TagReader {
            public:
                Id3v1Reader(TagLib::ID3v1::Tag * tag = 0);
                ~Id3v1Reader() { }
                Tag read();
        };

        class Id3v2Reader : public TagReader {
            public:
                Id3v2Reader(TagLib::ID3v2::Tag * tag = 0);
                ~Id3v2Reader() { }
                Tag read();
        };

        class XiphReader : public TagReader {
            public:
                XiphReader(TagLib::Ogg::XiphComment * tag = 0);
                ~XiphReader() { }
                Tag read();
        };
    }
}

#endif
