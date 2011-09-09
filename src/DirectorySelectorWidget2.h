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

#ifndef DIRECTORYSELECTORWIDGET2_H
#define DIRECTORYSELECTORWIDGET2_H

#include <QWidget>

class QFileSystemModel;
class QLineEdit;
class QModelIndex;
class QTreeView;

class FileSystemProxyModel;

namespace Ui {
	class DirectorySelector;
};

class DirectorySelectorWidget2 : public QWidget {
	Q_OBJECT

	public:
		DirectorySelectorWidget2(QWidget * parent=0);
		QString path() const;

	signals:
		void pathSelected(const QString &);

	public slots:
		void setPath(const QString &);
		void setRootPath(const QString &);
		void setRootIndex(const QModelIndex &);

	private slots:
		void changePath(const QModelIndex &);
		void changeToTypedPath();
		void goDirectoryUp();

		void showTreeContextMenu(const QPoint &);

	protected:
		bool eventFilter(QObject * object, QEvent * event);

	private:
		Ui::DirectorySelector * _ui;
		QFileSystemModel * _model;
		FileSystemProxyModel * _proxy;
};

#endif
