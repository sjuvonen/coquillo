
#include <QSettings>

#include "MetaDataImage.h"

QHash<qint16, QImage> MetaDataImage::s_cache;
QHash<qint16, QImage> MetaDataImage::s_small;

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

	QSettings s;

	if (s.value("ScaleCovers").toBool()) {
		const QSize size = QSize(
			s.value("ScaleWidth").toInt(),
			s.value("ScaleHeight").toInt()
		);
		
		s_cache[_id] = image.scaled(size, Qt::KeepAspectRatio);
	} else {
		s_cache[_id] = image;
		s_small[_id] = image.scaled(QSize(96, 96), Qt::KeepAspectRatio);
	}
}

QImage MetaDataImage::image() const {
	return s_cache.contains(_id) ? s_cache[_id] : QImage();
}

QImage MetaDataImage::small() const {
	if (s_small.contains(_id))
		return s_small[_id];

	if (s_cache.contains(_id))
		return s_cache[_id];

	return QImage();
}

