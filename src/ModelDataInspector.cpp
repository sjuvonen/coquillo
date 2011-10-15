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

#include <QAbstractItemModel>
#include <QFileInfo>
#include <QRegExp>
#include <QSettings>
#include <QStringList>

#include <metadata/MetaDataModel.h>

#include "ModelDataInspector.h"
#include "globals.h"

#include <QDebug>

ModelDataInspector::ModelDataInspector(QObject * parent=0)
	: QObject(parent), _model(0) {

}

void ModelDataInspector::setModel(QAbstractItemModel * model) {

	if (_model)  {
		disconnect(_model, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
			this, SLOT(modelDataChanged(const QModelIndex &, const QModelIndex &)));

		disconnect(_model, SIGNAL(rowsInserted(const QModelIndex &, int, int)),
			this, SLOT(modelRowsInserted(const QModelIndex &, int, int)));
	}

	connect(model, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
		SLOT(modelDataChanged(const QModelIndex &, const QModelIndex &)));

	connect(model, SIGNAL(rowsInserted(const QModelIndex &, int, int)),
		SLOT(modelRowsInserted(const QModelIndex &, int, int)));

	_model = model;
}

void ModelDataInspector::modelDataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight) {
	checkData(topLeft.row(), bottomRight.row(), topLeft.parent());
}

void ModelDataInspector::modelRowsInserted(const QModelIndex & parent, int start, int end) {
	checkData(start, end, parent);
}

void ModelDataInspector::checkData(int firstRow, int lastRow, const QModelIndex & parent) {
	disconnect(_model, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
		this, SLOT(modelDataChanged(const QModelIndex &, const QModelIndex &)));

	for (int i = firstRow; i <= lastRow; i++) {
		const QModelIndex idx = _model->index(i, 0, parent);
		QMap<int, QString> fields;

		MetaDataModel * mdm = MetaDataModel::instance();
		
		for (int j = 0; j < mdm->columnCount(); j++)
			fields.insert(j, mdm->columnName(j));

		fields.remove( modelColumn("Disc") );
		fields.remove( modelColumn("Number") );
		fields.remove( modelColumn("MaxNumber") );
		fields.remove( modelColumn("Path") );
		fields.remove( modelColumn("Year") );
		fields.remove( modelColumn("Pictures") );

		if (Coquillo::safeFileNames) {
			const QModelIndex idx = _model->index(i, modelColumn("Path"));
			const QRegExp chars("[\"*:<>\?\\|]");

			QString path = idx.data(Qt::EditRole).toString();
			const QString dir = path.section('/', 0, -2);
			const QString name = path.section('/', -1).remove(chars);

			path = QString("%1/%2").arg(dir, name);

			_model->setData( _model->index(i, modelColumn("Path")), path );
		}

		if (Coquillo::fileExtensionCase) {
			const QModelIndex idx = _model->index(i, modelColumn("Path"));
			QString path = idx.data(Qt::EditRole).toString();
			const QString suffix = path.section('.', -1);

			path.chop(suffix.length());

			if (Coquillo::fileExtensionCase == 1)
				path.append(suffix.toLower());
			else
				path.append(suffix.toUpper());

			_model->setData(idx, path);
		}

		if (Coquillo::removeDiscFromAlbumName) {
			const QModelIndex idx = _model->index(i, modelColumn("Album"));
			QRegExp r("\\s*[\\[\\(](?:disc|cd)\\s*(\\d+)[\\]\\)]\\s*$");

			r.setCaseSensitivity(Qt::CaseInsensitive);

			QString albumName = idx.data(Qt::EditRole).toString();

			if (r.indexIn(albumName) != -1) {
				int disc = r.cap(1).toInt();
				albumName.remove(r);

				_model->setData(idx, albumName);
				_model->setData(idx.sibling(idx.row(), modelColumn("Disc")), disc);
			}



		}

		foreach (int col, fields.keys()) {
			/**
			 * Fields that begin with an underscore are internal informaation fields
			 * and must not not be altered.
			 **/

			const QModelIndex idx = _model->index(i, col);
			QString value = idx.data(Qt::EditRole).toString();

			if (Coquillo::trimWhiteSpace)
				value = value.trimmed().replace(QRegExp("\\s+"), QString(' '));

			_model->setData(idx, value);

		}

	}

	connect(_model, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
		this, SLOT(modelDataChanged(const QModelIndex &, const QModelIndex &)));
}
