#ifndef COQUILLO_CRAWLER_TAG_GENERIC_H
#define COQUILLO_CRAWLER_TAG_GENERIC_H

#include <QVariantHash>
#include "../types.hpp"

namespace TagLib {
    class Tag;
}

namespace Coquillo {
    namespace Crawler {
        namespace Tag {
            class Generic {
                public:
                    static quint16 imageId(const QImage & image);
                    Generic() {}
                    TagData read(const TagLib::Tag * tag) const;
                    virtual void write(TagLib::Tag * tag, const TagData & data);

                    /**
                     * Convert QImage to QByteArray
                     *
                     * @param image QImage to convert
                     * @param format Either a complete mime type or image type name such as 'PNG' or 'JPEG'.
                     */
                    static QByteArray imageToBytes(const QImage & img, const QString & format = "JPEG");

            };
        }
    }
}

#endif
