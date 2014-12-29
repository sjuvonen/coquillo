#ifndef COQUILLO_METADATA_TAG_ABSTRACTTAG_H
#define COQUILLO_METADATA_TAG_ABSTRACTTAG_H

#include <taglib/tag.h>

namespace Coquillo {
    namespace MetaData {
        namespace Container {
            class Default {
                public:
                    Default(TagLib::Tag * tag);
                    virtual ~Default() { }
                    virtual void write(const QVariantMap & data);
                    virtual QVariantMap read() const;

                protected:
                    TagLib::Tag * _tag;
            };
        }
    }
}

#endif
