#include "ImageCache.h"
#include "MetaDataImage.h"

MetaDataImage::MetaDataImage() : _id(-1), _type(0) {

}

MetaDataImage::MetaDataImage(const QImage & image) : _type(0) {
	_id = ImageCache::addImage(image);

}

MetaDataImage::MetaDataImage(const QImage & image, int type, const QString & description)
: _type(type), _description(description) {

	_id = ImageCache::addImage(image);
}

MetaDataImage::MetaDataImage(qint16 id, int type, const QString & description)
: _id(id), _type(type), _description(description) {

}


MetaDataImage::MetaDataImage(qint16 id, const QString & typeString, const QString & description)
	: _id(id), _type(-1), _typeString(typeString), _description(description) {

}

MetaDataImage::MetaDataImage(const QImage & image, const QString & typeString, const QString & description)
	: _type(-1), _typeString(typeString), _description(description) {
	_id = ImageCache::addImage(image);
}

QImage MetaDataImage::image() const {
	return ImageCache::image(_id);
}

QString MetaDataImage::description() const {
	return _description;
}

QString MetaDataImage::typeString() const {
	return _typeString;
}

int MetaDataImage::type() const {
	return _type;
}

qint16 MetaDataImage::id() const {
	return _id;
}
