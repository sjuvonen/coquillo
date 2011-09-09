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

#ifndef MODELDATAINSPECTOR_H
#define MODELDATAINSPECTOR_H

#include <QObject>
#include <QModelIndex>

class QAbstractItemModel;

class ModelDataInspector : public QObject {
	Q_OBJECT

	public:
		ModelDataInspector(QObject * parent);
		void setModel(QAbstractItemModel *);

	public slots:
		void checkData(int firstRow, int lastRow, const QModelIndex & parent=QModelIndex());

	private slots:
		void modelDataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight);
		void modelRowsInserted(const QModelIndex & parent, int start, int end);

	private:
		QAbstractItemModel * _model;
};

#endif
