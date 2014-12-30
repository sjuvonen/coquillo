#ifndef COQUILLO_METADATA_IMAGECACHE_H
#define COQUILLO_METADATA_IMAGECACHE_H

#include <QHash>
#include <QImage>

namespace Coquillo {
    namespace MetaData {
        class ImageCache {
            public:
                static ImageCache * instance();

                int insert(const QImage & image);
                void resize(int id, const QSize & size);

                bool contains(int id) const { return _images.contains(id); }
                const QImage image(int id) const { return _images[id]; }
                void replace(int id, const QImage & image) { _images.insert(id, image); }
                int count() const { return _images.count(); }

            private:
                static ImageCache * s_instance;
                QHash<int, QImage> _images;

        };
    }
}

#endif
