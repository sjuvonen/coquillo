#ifndef COQUILLO_METADATA_IMAGE_H
#define COQUILLO_METADATA_IMAGE_H

#include <QImage>
#include <QString>

namespace Coquillo {
    namespace MetaData {
        class ImageCache;

        class Image {
            public:
                Image();

                bool isNull() const;
                const QImage scaled(const QSize & size) const;
                const QImage source() const;
                void setSource(const QImage & image);

                int height() const;
                int width() const;

                inline QString description() const { return _description; }
                inline void setDescription(const QString & d)  { _description = d; }
                inline int type() const { return _type; }
                inline void setType(int type) { _type = type; }

                inline QString mimeType() const { return _mime; }
                inline void setMimeType(const QString & m) { _mime = m; }
                inline quint16 id() const { return _id; }

            private:
                static ImageCache * cache();

                quint16 _id;
                int _type;
                QString _description;
                QString _mime;
        };

        typedef QList<Image> ImageList;
    }
}

#endif
