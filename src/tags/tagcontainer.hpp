#ifndef COQUILLO_TAGS_CONTAINER_H
#define COQUILLO_TAGS_CONTAINER_H

#include "image.hpp"
#include "tag.hpp"

namespace Coquillo {
    namespace Tags {
        class Container {
            public:
                Container();
                Container(const QString & path);

                void addTag(const Tag & tag);
                void setPrimaryTag(const QString & id);
                QVariant value(const QString & field) const;

                void setImages(const QList<Image> & images);

                inline QString primaryTag() const { return _primary; }
                inline QString path() const { return _path; }
                inline void setPath(const QString & path) { _path = path; }

                inline uint id() const { return _id; }
                inline bool isNull() const { return _path.isNull(); }
                inline const QList<Image> images() const { return _images; }
                inline QList<Image> & images() { return _images; }
                inline int imageCount() const { return _image_count; }
                inline void setImageCount(int count) { _image_count = count; }

                bool hasTag(const QString & id) const;
                const Tag tag(const QString & id) const;
                Tag & tag(const QString & id);
                QList<Tag> tags() const;
                QStringList tagNames() const;

                void addImage(const Image & image);
                const Image image(int pos) const { return _images.value(pos); }
                Image & image(int pos) { return _images[pos]; }

            private:
                uint _id;
                QString _path;
                QString _primary;
                QHash<QString, Tag> _tags;
                QList<Image> _images;
                int _image_count;
        };
    }
}

Q_DECLARE_METATYPE(Coquillo::Tags::Container);

#endif
