#ifndef COQUILLO_METADATA_TAG_DUMMY_H
#define COQUILLO_METADATA_TAG_DUMMY_H

#include "abstracttag.h"

namespace TagLib {
    class Tag;
}

namespace Coquillo {
    namespace MetaData {
        namespace Container {
            class Dummy : public AbstractTag {
                public:
                    Dummy(TagLib::Tag * tag);
                    void write(const QVariantMap & data);
                    QVariantMap read() const;
            };
        }
    }
}

#endif
