
#include "image.h"
#include "imagecache.h"

namespace Coquillo {
    namespace MetaData {
        Image::Image()
        : _id(0), _type(0) {

        }

        bool Image::isNull() const {
            return !cache()->contains(id());
        }

        int Image::height() const {
            return source().height();
        }

        const QImage Image::scaled(const QSize & size) const {
            return cache()->scaled(id(), size);
        }

        const QImage Image::source() const {
            return cache()->image(id());
        }

        void Image::setSource(const QImage & image) {
            _id = cache()->insert(image);
        }

        int Image::width() const {
            return source().width();
        }

        ImageCache * Image::cache() {
            return ImageCache::instance();
        }
    }
}
