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

#ifndef TABLEVIEWMOD_H
#define TABLEVIEWMOD_H

#include <QTableView>

class TableViewMod : public QTableView {
	Q_OBJECT

	public:
		TableViewMod(QWidget * parent=0);

	signals:
		void currentSelectionChanged(const QModelIndexList & rows);

	public slots:
		void selectNextIndex();
		void selectInverse();
		void selectPreviousIndex();

	protected slots:
		void selectionChanged(const QItemSelection & selected, const QItemSelection & deselected);

	protected:
		void mousePressEvent(QMouseEvent *);

};

#endif