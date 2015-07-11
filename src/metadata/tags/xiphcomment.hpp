#ifndef COQUILLO_METADATA_TAG_XIPHCOMMENT_H
#define COQUILLO_METADATA_TAG_XIPHCOMMENT_H

#include "default.hpp"

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
                    void write(const Tag & data);
                    Tag read() const;
                    QList<Image> readImages() const;

                private:
                    static Image parseImage(const QByteArray & data);
                    QList<Image> readLegacyImages() const;
            };
        }
    }
}

#endif
