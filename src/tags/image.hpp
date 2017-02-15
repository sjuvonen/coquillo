#ifndef COQUILLO_TAGS_IMAGE_H
#define COQUILLO_TAGS_IMAGE_H

#include <QImage>
#include <QString>

namespace Coquillo {
    namespace Tags {
        class ImageCache;

        class Image {
            public:
                static Image fromValues(const QVariantHash & values);

                Image();
                Image(const QImage & source);
                Image(const QImage & source, quint16 id);

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

                bool operator==(const Image & other) const;

            private:
                static ImageCache * cache();

                quint16 _id;
                int _type;
                QString _description;
                QString _mime;
        };
    }
}

Q_DECLARE_METATYPE(Coquillo::Tags::Image)

#endif
