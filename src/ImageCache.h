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

#ifndef IMAGECACHE
#define IMAGECACHE

#include <QMap>
#include <QObject>
#include <QSize>

class QImage;

class ImageCache : public QObject {
	Q_OBJECT

	public:
		ImageCache(QObject * parent);

		static QImage image(qint16 id);
		static qint16 addImage(const QImage & image);
		static void setScaleSize(const QSize & size);
		static QSize scaleSize() { return QSize(_w, _h); }

	public slots:
		static void clear();

	private:
		static QMap<qint16, const QImage *> _images;
		static QMap<qint16, const QImage *> _scaled;

		static int _w;
		static int _h;
};

#endif