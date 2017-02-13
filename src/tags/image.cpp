
#include <QVariant>

#include "image.hpp"
#include "imagecache.hpp"

#include <QDebug>

namespace Coquillo {
    namespace Tags {
        Image Image::fromValues(const QVariantHash & values) {
            Image image(values["data"].value<QImage>(), values["id"].toUInt());
            image.setDescription(values.value("description").toString());
            image.setMimeType(values.value("mime").toString());
            image.setType(values.value("type").toInt());
            return image;
        }

        Image::Image()
        : _id(0), _type(0) {

        }

        Image::Image(const QImage & source, quint16 id)
        : _id(id) {
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
            cache()->insert(image, _id);
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
