
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

        QImage Image::source() const {
            return cache()->image(id());
        }

        void Image::setSource(const QImage & image) {
            _id = cache()->insert(image);
        }

        ImageCache * Image::cache() const {
            return ImageCache::instance();
        }
    }
}
