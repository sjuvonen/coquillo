#ifndef COQUILLO_METADATA_TAG_XIPHCOMMENT_H
#define COQUILLO_METADATA_TAG_XIPHCOMMENT_H

#include "abstracttag.h"

namespace TagLib {
    namespace Ogg {
        class XiphComment;
    }
}

namespace Coquillo {
    namespace MetaData {
        namespace Container {
            class XiphComment : public AbstractTag {
                public:
                    XiphComment(TagLib::Ogg::XiphComment * tag);
                    void write(const QVariantMap & data);
                    QVariantMap read() const;
            };
        }
    }
}

#endif
