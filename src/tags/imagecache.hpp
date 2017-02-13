#ifndef COQUILLO_METADATA_IMAGECACHE_H
#define COQUILLO_METADATA_IMAGECACHE_H

#include <QHash>
#include <QImage>

namespace Coquillo {
    namespace Tags {
        typedef QHash<QString, QImage> SizeCache;

        /*
         * NOTE: One option for tracking scaled images (for marking changed metadata)
         * is to store the original image size in Container and then compare that
         * to the size of referenced QImage.
         */
        class ImageCache {
            public:
                static ImageCache * instance();
                quint16 insert(const QImage & image, quint16 id = 0);
                void resize(quint16 id, const QSize & size);

                bool contains(quint16 id) const { return _images.contains(id); }
                const QImage image(quint16 id) const { return _images[id]; }
                const QImage scaled(quint16 id, const QSize & size);
                void replace(quint16 id, const QImage & image) { _images.insert(id, image); }
                int count() const { return _images.count(); }
                const QList<quint16> ids() const { return _images.keys(); }

                void test() const;

            private:
                static ImageCache * s_instance;
                QHash<quint16, QImage> _images;
                QHash<quint16, SizeCache> _scaled;
        };
    }
}

#endif
