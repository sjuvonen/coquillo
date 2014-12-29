#ifndef COQUILLO_METADATA_TAG_XIPHCOMMENT_H
#define COQUILLO_METADATA_TAG_XIPHCOMMENT_H

#include "default.h"

namespace TagLib {
    namespace Ogg {
        class XiphComment;
    }
}

namespace Coquillo {
    namespace MetaData {
        namespace Container {
            class XiphComment : public Default {
                public:
                    XiphComment(TagLib::Ogg::XiphComment * tag);
                    void write(const QVariantMap & data);
                    QVariantMap read() const;
            };
        }
    }
}

#endif
