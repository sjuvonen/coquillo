#ifndef COQUILLO_METADATA_TAG_ID3V2_H
#define COQUILLO_METADATA_TAG_ID3V2_H

#include "abstracttag.h"

namespace TagLib {
    namespace ID3v2 {
        class Tag;
    }
}

namespace Coquillo {
    namespace MetaData {
        namespace Container {
            class Id3v2 : public AbstractTag {
                public:
                    Id3v2(TagLib::ID3v2::Tag * tag);
                    void write(const QVariantMap & data);
                    QVariantMap read() const;
            };
        }
    }
}

#endif
