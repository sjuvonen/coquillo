#ifndef COQUILLO_CRAWLER_TAG_ID3V2TAG_H
#define COQUILLO_CRAWLER_TAG_ID3V2TAG_H

#include <QVariantHash>

namespace TagLib {
    namespace ID3v2 {
        class Tag;
    }
}

namespace Coquillo {
    namespace Crawler {
        namespace Tag {
            class Id3v2Tag {
                public:
                    QVariantHash read(const TagLib::ID3v2::Tag * tag) const;
            };
        }
    }
}
#endif
