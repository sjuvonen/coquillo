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

#ifndef METADATAMODEL_H
#define METADATAMODEL_H

#include <QStandardItemModel>

#include "def_MetaData.h"
#include "MetaDataImage.h"

class MetaDataModel : public QStandardItemModel {
	Q_OBJECT

	public:
		MetaDataModel(QObject * parent=0);

		QVariant data(const QModelIndex & idx, int role=Qt::DisplayRole) const;
		MetaData metaData(int row, bool withPictures=false) const;
		MetaData metaData(const QModelIndex & idx, bool withPictures=false) const;

		QList<MetaData> modifiedMetaData(bool withPictures=false) const;

		bool setData(const QModelIndex & idx, const QVariant & value, int role=Qt::EditRole);

	public slots:
		void setRootDirectory(const QString & path);
		void addMetaData(const MetaData &);
		void clearContents();
		void saveChanges();
		void undoChanges();

	signals:
		void metaDataStateChanged(bool isModified);
		void metaDataModified(const QModelIndex & idx);

	private:
		QString _root;
};

#endif
