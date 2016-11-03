#ifndef COQUILLO_CRAWLER_TAG_ID3V2TAG_H
#define COQUILLO_CRAWLER_TAG_ID3V2TAG_H

#include "../imagetype.hpp"
#include "generic.hpp"

namespace TagLib {
    namespace ID3v2 {
        class Tag;
    }
}

namespace Coquillo {
    namespace Crawler {
        namespace Tag {
            class Id3v2 : public Generic {
                public:
                    void write(TagLib::ID3v2::Tag * tag, const TagData & values);
                    TagData read(const TagLib::ID3v2::Tag * tag) const;
                    ImageDataList readImages(const TagLib::ID3v2::Tag * tag) const;
            };
        }
    }
}
#endif
