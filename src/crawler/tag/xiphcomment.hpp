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
                    void write(TagLib::Ogg::XiphComment * tag, const TagData & data);
                    TagData read(const TagLib::Ogg::XiphComment * tag) const;
                    int imageCount(const TagLib::Ogg::XiphComment * tag) const;

                    ImageDataList readImages(const TagLib::Ogg::XiphComment * tag) const;
                    ImageDataList readLegacyImages(const TagLib::Ogg::XiphComment * tag) const;

                    #if TAGLIB_MINOR_VERSION >= 7 || TAGLIB_MAJOR_VERSION > 1
                    ImageDataList readFlacImages(const TagLib::List<TagLib::FLAC::Picture *> pictures) const;
                    #endif

                private:
                    static ImageData parseImage(const QByteArray & data);
            };
        }
    }
}

#endif
