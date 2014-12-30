
#include "imagecache.h"

namespace Coquillo {
    namespace MetaData {
        ImageCache * ImageCache::s_instance = 0;

        ImageCache * ImageCache::instance() {
            if (!s_instance) {
                s_instance = new ImageCache;
            }
            return s_instance;
        }

        int ImageCache::insert(const QImage & image) {
            const auto bits = reinterpret_cast<const char*>(image.bits());
            int id = qChecksum(bits, image.byteCount());
            if (id and !_images.contains(id)) {
                _images[id] = image;
            }
            return id;
        }

        void ImageCache::resize(int id, const QSize & size) {
            if (contains(id)) {
                const QImage image = this->image(id);
                if (image.width() > size.width() or image.height() > size.height()) {
                    const QImage scaled = image.scaled(size, Qt::KeepAspectRatio);
                    replace(id, scaled);
                }
            }
        }
    }
}
