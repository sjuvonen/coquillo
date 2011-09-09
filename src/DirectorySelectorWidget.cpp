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

#include <QFileSystemModel>
#include <QHeaderView>
#include <QLineEdit>
#include <QMenu>
#include <QModelIndex>
#include <QMouseEvent>
#include <QToolButton>
#include <QTreeView>
#include <QVBoxLayout>

#include "DirectorySelectorWidget.h"
#include "FileSystemProxyModel.h"

#include "uih/ui_DirectorySelector.h"

#include <QDebug>

DirectorySelectorWidget::DirectorySelectorWidget(QWidget * parent) : QWidget(parent) {

	_ui = new Ui::DirectorySelector;
	_ui->setupUi(this);

	_model = new QFileSystemModel(this);
	_model->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

	_proxy = new FileSystemProxyModel(this);
	_proxy->setSourceModel(_model);

	_ui->directories->setModel(_proxy);

	_ui->dirUp->setIcon(style()->standardIcon(QStyle::SP_FileDialogToParent));

	_ui->directories->viewport()->installEventFilter(this);
}

QString DirectorySelectorWidget::path() const {
	return _model->filePath(_ui->directories->currentIndex());
}

void DirectorySelectorWidget::setPath(const QString & path) {
	const QModelIndex i = _model->index(path, 0);

	_ui->directories->setCurrentIndex( _proxy->mapFromSource(i) );
	_ui->directories->expand(i);
	_ui->location->setText(path);
}

void DirectorySelectorWidget::setRootPath(const QString & path) {
	_model->setRootPath(path);

	const QModelIndex idx = _proxy->mapFromSource( _model->index(path) );
	_ui->directories->setRootIndex(idx);
}

void DirectorySelectorWidget::setRootIndex(const QModelIndex & idx) {
	if (!idx.isValid())
		return;

	setRootPath( _model->filePath(_proxy->mapToSource(idx)) );
}

void DirectorySelectorWidget::changePath(const QModelIndex & idx) {
	const QString path = _model->filePath( _proxy->mapToSource(idx) );
	_ui->location->setText(path);

	emit pathSelected(path);
}

void DirectorySelectorWidget::changeToTypedPath() {
	const QString path = _ui->location->text();

	setRootPath(path);
	setPath(path);
}

void DirectorySelectorWidget::goDirectoryUp() {
	const QModelIndex idx = _model->index( _model->rootPath() ).parent();

	if (idx.isValid()) {
		setRootIndex( _proxy->mapFromSource(idx) );

	}
}





void DirectorySelectorWidget::showTreeContextMenu(const QPoint & point) {
	QMenu menu(this);
	QAction * setRoot = menu.addAction(tr("Set as root"));

	QAction * a = menu.exec( _ui->directories->mapToGlobal(point) );

	if (a == setRoot) {
		setRootIndex( _ui->directories->currentIndex() );
	}
}





bool DirectorySelectorWidget::eventFilter(QObject * object, QEvent * event) {

	if (object == _ui->directories->viewport() && event->type() == QEvent::MouseButtonRelease) {
		QMouseEvent * e = dynamic_cast<QMouseEvent*>(event);
		const QModelIndex idx = _ui->directories->indexAt(e->pos());

		if (e && e->button() == Qt::LeftButton &&
			e->pos().x() >= _ui->directories->visualRect(idx).x() + _ui->directories->indentation()) {
			_ui->directories->setExpanded( idx, !_ui->directories->isExpanded(idx) );
			return false;
		}
	}

	return QWidget::eventFilter(object, event);
}