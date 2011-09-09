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
