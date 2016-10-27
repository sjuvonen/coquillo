#ifndef COQUILLO_CRAWLER_TAG_XIPHCOMMENT_H
#define COQUILLO_CRAWLER_TAG_XIPHCOMMENT_H

namespace TagLib {
    namespace Ogg {
        class XiphComment;
    }
}

namespace Coquillo {
    namespace Crawler {
        namespace Tag {
            class XiphComment {
                public:
                    QVariantHash read(const TagLib::Ogg::XiphComment * tag) const;
                    // void write(const QVariantHash & data);
                    // QList<Image> readImages() const;

                private:
                    // static Image parseImage(const QByteArray & data);
                    // QList<Image> readLegacyImages() const;
            };
        }
    }
}

#endif
