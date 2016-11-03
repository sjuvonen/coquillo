#ifndef COQUILLO_CRAWLER_TAG_XIPHCOMMENT_H
#define COQUILLO_CRAWLER_TAG_XIPHCOMMENT_H

#include "../imagetype.hpp"
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
                    void write(TagLib::Ogg::XiphComment * tag, const QVariantHash & data);
                    QVariantHash read(const TagLib::Ogg::XiphComment * tag) const;
                    ImageDataList readImages(const TagLib::Ogg::XiphComment * tag) const;

                private:
                    static ImageData parseImage(const QByteArray & data);
                    ImageDataList readLegacyImages(const TagLib::Ogg::XiphComment * tag) const;
            };
        }
    }
}

#endif
