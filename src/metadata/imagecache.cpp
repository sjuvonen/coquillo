
#include <QDebug>
#include "imagecache.hpp"

namespace Coquillo {
    namespace MetaData {
        ImageCache * ImageCache::s_instance = 0;

        ImageCache * ImageCache::instance() {
            if (!s_instance) {
                s_instance = new ImageCache;
            }
            return s_instance;
        }

        quint16 ImageCache::insert(const QImage & image) {
            const auto bits = reinterpret_cast<const char*>(image.bits());
            quint16 id = qChecksum(bits, image.byteCount());
            if (id and !_images.contains(id)) {
                _images[id] = image;
            }
            return id;
        }

        void ImageCache::resize(quint16 id, const QSize & size) {
            if (contains(id)) {
                const QImage image = this->image(id);
                if (image.width() > size.width() or image.height() > size.height()) {
                    replace(id, scaled(id, size));
                }
            }
        }

        const QImage ImageCache::scaled(quint16 id, const QSize & size) {
            const QString key = QString("%1x%2").arg(size.width()).arg(size.height());
            if (contains(id)) {
                if (!_scaled[id].contains(key)) {
                    _scaled[id][key] = image(id).scaled(size, Qt::KeepAspectRatio,
                        Qt::SmoothTransformation);
                }
                return _scaled[id][key];
            } else {
                return QImage();
            }
        }

        void ImageCache::test() const {
            qDebug() << "keys:" << _images.keys();
            foreach (const QImage & image, _images.values()) {
                qDebug() << "image:" << image.isNull();
            }
        }
    }
}
