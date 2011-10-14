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

#include "MetaDataModel.h"

#include "globals.h"

#include <QDebug>

MetaDataModel::MetaDataModel(QObject * parent) : QStandardItemModel(parent) {

	insertColumns(0, g_fieldNames.count());

	QStringList labels = g_fieldNames.values();
	labels.replace(modelColumn("Number"), "#");
	setHorizontalHeaderLabels(labels);
}

QVariant MetaDataModel::data(const QModelIndex & idx, int role) const {
	switch (role) {
		case Qt::BackgroundRole: {
			if (!Coquillo::dimSubdirectoriesBackground)
				return QStandardItemModel::data(idx, role);

			static QColor c(Qt::white);
			c.setAlpha(128);

			return c.darker( 100 + 20 * idx.sibling(idx.row(), modelColumn("Path")).data().toString().count('/') );
		}

		case Qt::DisplayRole:
			if (idx.column() == modelColumn("Path")) {
				const QVariant v = QStandardItemModel::data(idx, role);
				const QRegExp rx( QString("^%1").arg( QRegExp::escape(_root)) );

				if (!v.isNull())
					return v.toString().remove(rx);
			} else
				return QStandardItemModel::data(idx, role);

		case Qt::ForegroundRole:
			if (!idx.sibling(idx.row(), 0).data(Coquillo::RowModifiedRole).isNull())
				return Qt::red;

		case Qt::TextAlignmentRole:
			if (idx.column() == modelColumn("Number"))
				return Qt::AlignCenter;
			else
				return QStandardItemModel::data(idx, role);

		default:
			return QStandardItemModel::data(idx, role);
	}
}

bool MetaDataModel::setData(const QModelIndex & idx, const QVariant & value, int role) {

	//  qDebug() << "set" << idx.data(Qt::EditRole).toString() << value.toString();
	
	if (role == Qt::EditRole && idx.column() != g_fieldNames.key("Pictures")
		&& idx.data(role).toString() == value.toString())
		return false;

	if (role == Qt::EditRole && !idx.data(Qt::EditRole).isNull() && idx.data(Coquillo::OriginalDataRole).isNull()) {
		QStandardItemModel::setData(idx, idx.data(Qt::EditRole), Coquillo::OriginalDataRole);
		QStandardItemModel::setData(idx.sibling(idx.row(), 0), true, Coquillo::RowModifiedRole);

		emit metaDataStateChanged(true);
	}

	return QStandardItemModel::setData(idx, value, role);
}

MetaData MetaDataModel::metaData(int row, bool withPictures) const {
	Q_UNUSED(withPictures)

	MetaData data;

	for (int i = 0; i < columnCount(); i++)
		data.insert( g_fieldNames.value(i), index(row, i).data(Qt::EditRole) );

	return data;
}

MetaData MetaDataModel::metaData(const QModelIndex & idx, bool withPictures) const {
	return metaData(idx.row(), withPictures);
}

QList<MetaData> MetaDataModel::modifiedMetaData(bool withPictures) const {
	Q_UNUSED(withPictures)

	QList<MetaData> data;

	for (int row = 0; row < rowCount(); row++) {
		if (index(row, 0).data(Coquillo::RowModifiedRole).isNull())
			continue;

		MetaData item;

		for (int col = 0; col < columnCount(); col++) {
			const QModelIndex idx = index(row, col);

			if (!idx.data(Coquillo::OriginalDataRole).isNull())
				item.insert( g_fieldNames.value(col), idx.data(Qt::EditRole) );
		}

		const QModelIndex pathIdx = index(row, g_fieldNames.key("Path"));

		item.insert("Path", pathIdx.data(Qt::EditRole));

		if (!pathIdx.data(Coquillo::OriginalDataRole).isNull())
			item.insert("_OldPath", pathIdx.data(Coquillo::OriginalDataRole));

		if (item.contains("MaxNumber"))
			item.insert("Number", index(row, g_fieldNames.key("Number")).data());

		data << item;
	}

	return data;
}





void MetaDataModel::setRootDirectory(const QString & path) {
	if (_root == path)
		return;

	_root = path;

	if (!_root.isEmpty() && _root.right(1) != "/")
		_root.append('/');

	emit dataChanged(index(0, 0), index(rowCount()-1, columnCount()-1));
}

void MetaDataModel::addMetaData(const MetaData & data) {
	QList<QStandardItem*> items;

	foreach (int key, g_fieldNames.keys()) {
		const QString field = g_fieldNames.value(key);

		QStandardItem * item = new QStandardItem;
		item->setDropEnabled(false);

		if (field != "Pictures") {
			const QVariant value = data.value(field);
			item->setData( value, Qt::EditRole );
		}

		items << item;
	}

	appendRow(items);
}

void MetaDataModel::clearContents() {
	removeRows(0, rowCount());
}

void MetaDataModel::saveChanges() {
	for (int row = 0; row < rowCount(); row++) {
		if (index(row, 0).data(Coquillo::RowModifiedRole).isNull())
			continue;

		for (int col = 0; col < columnCount(); col++)
			setData(index(row, col), QVariant(), Coquillo::OriginalDataRole);

		setData(index(row, 0), QVariant(), Coquillo::RowModifiedRole);
	}

	emit metaDataStateChanged(false);
}

void MetaDataModel::undoChanges() {
	for (int row = 0; row < rowCount(); row++) {
		if (index(row, 0).data(Coquillo::RowModifiedRole).isNull())
			continue;

		for (int col = 0; col < columnCount(); col++) {
			const QModelIndex idx = index(row, col);

			if (!idx.data(Coquillo::OriginalDataRole).isNull()) {
				setData(idx, idx.data(Coquillo::OriginalDataRole), Qt::EditRole);
				setData(idx, QVariant(), Coquillo::OriginalDataRole);
			}

		}

		setData(index(row, 0), QVariant(), Coquillo::RowModifiedRole);


		// This signal needs to be emitted because we might not change data for each cell,
		// yet they are all marked as changed (red text) in the item view.
		emit dataChanged(index(row, 1), index(row, columnCount()-1));
	}

	emit metaDataStateChanged(false);
}

