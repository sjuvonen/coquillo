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

#ifndef METADATAIMAGE_H
#define METADATAIMAGE_H

#include <QImage>
#include <QMetaType> // Needed for Q_DECLARE_METATYPE()
#include <QString>

class MetaDataImage {
	public:
		MetaDataImage();
		MetaDataImage(const QImage & image);
		MetaDataImage(const QImage & image, int type, const QString & description);
		MetaDataImage(qint16 id, int type, const QString & description);
		MetaDataImage(qint16 id, const QString & typeString, const QString & description);
		MetaDataImage(const QImage & image, const QString & typeString, const QString & description);

		int width() const { return image().width(); }
		int height() const { return image().height(); }
		QSize size() const { return image().size(); }

		QImage image() const;
		QString description() const;
		QString typeString() const;
		int type() const;
		qint16 id() const;

	private:
		// QImage _image;
		qint16 _id;
		int _type;
		QString _typeString;
		QString _description;
};

Q_DECLARE_METATYPE(MetaDataImage)

#endif