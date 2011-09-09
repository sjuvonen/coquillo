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

#include <QMouseEvent>

#include "TableViewMod.h"

TableViewMod::TableViewMod(QWidget * parent) : QTableView(parent) {


}

void TableViewMod::selectNextIndex() {
	const QModelIndex idx = model()->index( currentIndex().row()+1, 0);
	setCurrentIndex( idx.isValid() ? idx : model()->index(0, 0) );
}

void TableViewMod::selectInverse() {
	const QModelIndex tl = model()->index(0, 0);
	const QModelIndex br = model()->index(model()->rowCount()-1, model()->columnCount()-1);
	const QItemSelection s(tl, br);

	selectionModel()->select(s, QItemSelectionModel::Toggle);
}

void TableViewMod::selectPreviousIndex() {
	const QModelIndex idx = model()->index( currentIndex().row()-1, 0);
	setCurrentIndex( idx.isValid() ? idx : model()->index(model()->rowCount()-1, 0) );
}



void TableViewMod::mousePressEvent(QMouseEvent * e) {
	if (!indexAt(e->pos()).isValid())
		return;

	QTableView::mousePressEvent(e);
}




void TableViewMod::selectionChanged(const QItemSelection & selected, const QItemSelection & deselected) {
	QTableView::selectionChanged(selected, deselected);
	emit currentSelectionChanged(selectionModel()->selectedRows());
}
