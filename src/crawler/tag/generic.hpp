#ifndef COQUILLO_CRAWLER_TAG_GENERIC_H
#define COQUILLO_CRAWLER_TAG_GENERIC_H

#include <QVariantHash>
#include "../types.hpp"

namespace TagLib {
    class Tag;
}

namespace Coquillo {
    namespace Crawler {
        namespace Tag {
            class Generic {
                public:
                    Generic() {}
                    TagData read(const TagLib::Tag * tag) const;
                    virtual void write(TagLib::Tag * tag, const TagData & data);

            };
        }
    }
}

#endif
