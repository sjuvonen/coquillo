#ifndef COQUILLO_METADATA_TAG_ABSTRACTTAG_H
#define COQUILLO_METADATA_TAG_ABSTRACTTAG_H

#include <taglib/tag.h>

namespace Coquillo {
    namespace MetaData {
        namespace Container {
            class AbstractTag {
                public:
                    AbstractTag(TagLib::Tag * tag);
                    virtual ~AbstractTag() { }
                    virtual void write(const QVariantMap & data) = 0;
                    virtual QVariantMap read() const = 0;

                protected:
                    QVariantMap readCommon() const;
                    TagLib::Tag * _tag;
            };
        }
    }
}

#endif
