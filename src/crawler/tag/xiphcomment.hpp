#ifndef COQUILLO_CRAWLER_TAG_XIPHCOMMENT_H
#define COQUILLO_CRAWLER_TAG_XIPHCOMMENT_H

#include "generic.hpp"

namespace TagLib {
    namespace Ogg {
        class XiphComment;
    }
}

namespace Coquillo {
    namespace Crawler {
        namespace Tag {
            class XiphComment : public Generic {
                public:
                    QVariantHash read(const TagLib::Ogg::XiphComment * tag) const;
                    void write(TagLib::Ogg::XiphComment * tag, const QVariantHash & data);
                    // QList<Image> readImages() const;

                private:
                    // static Image parseImage(const QByteArray & data);
                    // QList<Image> readLegacyImages() const;
            };
        }
    }
}

#endif
