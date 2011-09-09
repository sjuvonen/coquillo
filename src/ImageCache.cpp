/***********************************************************************
* Copyright (c) 2011 Samu Juvonen <samu.juvonen@gmail.com>
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
************************************************************************/

#include <QBuffer>
#include <QByteArray>
#include <QImage>
#include <QSettings>

#include "ImageCache.h"

#include <QDebug>

QMap<qint16, const QImage *> ImageCache::_images = QMap<qint16, const QImage*>();
QMap<qint16, const QImage *> ImageCache::_scaled = QMap<qint16, const QImage*>();

int ImageCache::_w = 100;
int ImageCache::_h = 100;

ImageCache::ImageCache(QObject * parent=0)
	: QObject(parent) {

}


QImage ImageCache::image(qint16 id) {
	if (!_scaled.contains(id))
		return QImage();

	return *_scaled.value(id);
}

qint16 ImageCache::addImage(const QImage & image) {
	const char * bits = (const char *)(image.bits());
	int len = strlen(bits);
	qint16 id = qChecksum( bits, len );

	if (!_images.contains(id)) {
		const QImage * scaled;

		QSettings s;

		const QSize size = s.value("Pictures/ScaleSize").toSize();
		bool scale = s.value("Pictures/ScalePictures").toBool();

		if (scale)
			scaled = new QImage( image.scaled(size, Qt::KeepAspectRatio) );
		else
			scaled = new QImage(image);

		_images.insert(id, new QImage(image) );
		_scaled.insert(id, scaled);
	}

	return id;

}

void ImageCache::setScaleSize(const QSize & size) {
	_w = size.width();
	_h = size.height();
}

void ImageCache::clear() {
	qDeleteAll(_images);
	qDeleteAll(_scaled);

	_images.clear();
	_scaled.clear();
}
