#ifndef COQUILLO_CRAWLER_TAG_GENERIC_H
#define COQUILLO_CRAWLER_TAG_GENERIC_H

namespace TagLib {
    class Tag;
}

namespace Coquillo {
    namespace Crawler {
        namespace Tag {
            class Generic {
                public:
                    Generic() {}
                    QVariantHash read(const TagLib::Tag * tag) const;
                    virtual void write(const QVariantMap & data);

            };
        }
    }
}

#endif
