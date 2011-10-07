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

#ifndef PICTURESTAB_H
#define PICTURESTAB_H

#include <QMap>
#include <QWidget>

#include "MetaDataImage.h"

class QModelIndex;
class QStandardItemModel;

class MetaDataModel;

namespace Ui {
	class TagEditorPictures;
}

class PicturesTab : public QWidget {
	Q_OBJECT

	public:
		enum { PictureTypeRole = Qt::UserRole+1, PictureTypeStringRole, PictureDimensionsRole, PictureIdRole };

		PicturesTab(QWidget * parent=0);

		QList<MetaDataImage> pictures() const;

	signals:
		void applyPicturesToAll();
		void pictureAdded(const MetaDataImage &);
		void pictureChanged(const MetaDataImage &);
		void pictureRemoved(qint16 id);

	public slots:
		void clearPictures();
		void setPictures(const QList<MetaDataImage> & pics);

	private slots:
		void addPictures();
		void changePictureDescription(const QString & text);
		void changePictureType(int index);
		void changePictureType(const QString & customText);
		void deleteCurrentPicture();
		void savePicture();
		void selectedPictureChanged(const QModelIndex & idx);

	private:
		void addPictureToList(const MetaDataImage & img);

		Ui::TagEditorPictures * _ui;

		QStandardItemModel * _pictureModel;
		QMap<int, QString> _typeStrings;
};

#endif