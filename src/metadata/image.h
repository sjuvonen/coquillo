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
                QImage source() const;
                void setSource(const QImage & image);

                inline QString description() const { return _description; }
                inline void setDescription(const QString & d)  { _description = d; }
                inline int type() const { return _type; }
                inline void setType(int type) { _type = type; }

                inline QString mimeType() const { return _mime; }
                inline void setMimeType(const QString & m) { _mime = m; } 

            private:
                inline int id() const { return _id; }
                ImageCache * cache() const;

                int _id;
                int _type;
                QString _description;
                QString _mime;
        };

        typedef QList<Image> ImageList;
    }
}

#endif
