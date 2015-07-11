
#include "image.hpp"
#include "imagecache.hpp"

namespace Coquillo {
    namespace MetaData {
        Image::Image()
        : _id(0), _type(0) {

        }

        Image::Image(const QImage & source) {
            setSource(source);
            setMimeType("image/jpeg");
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

        bool Image::operator==(const Image & other) const {
            return _id == other._id
                and _type == other._type
                and _description == other._description
                and _mime == other._mime;
        }
    }
}
