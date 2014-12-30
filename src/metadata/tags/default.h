#ifndef COQUILLO_METADATA_TAG_ABSTRACTTAG_H
#define COQUILLO_METADATA_TAG_ABSTRACTTAG_H

#include <taglib/tag.h>
#include "metadata/tag.h"

namespace Coquillo {
    namespace MetaData {
        class Image;

        namespace Container {
            class Default {
                public:
                    Default(TagLib::Tag * tag);
                    virtual ~Default() { }
                    virtual void write(const Tag & data);
                    virtual Tag read() const;
                    virtual QList<Image> readImages() const;

                protected:
                    TagLib::Tag * _tag;
            };
        }
    }
}

#endif
