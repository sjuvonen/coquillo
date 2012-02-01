
#include "MetaDataImage.h"

QHash<qint16, QImage> MetaDataImage::s_cache;

MetaDataImage::MetaDataImage() : _type(0), _id(0) {
	
}

MetaDataImage::MetaDataImage(const QImage & image)
	: _type(0), _id(0) {

	setImage(image);
}

MetaDataImage::MetaDataImage(const QImage & image, int type, const QString & description)
	: _description(description), _type(type), _id(0) {

	setImage(image);
}

void MetaDataImage::setImage(const QImage & image) {
	const char * bits = (const char *)(image.bits());
	_id = qChecksum(bits, strlen(bits));

	if (s_cache.contains(_id))
		return;

	s_cache[_id] = image.scaled(QSize(100, 100), Qt::KeepAspectRatio);
}

QImage MetaDataImage::image() const {
	return s_cache.contains(_id) ? s_cache[_id] : QImage();
}