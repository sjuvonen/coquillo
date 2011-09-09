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
#include <QModelIndex>
#include <QTreeView>
#include <QVBoxLayout>

#include "DirectorySelectorWidget.h"

DirectorySelectorWidget::DirectorySelectorWidget(QWidget * parent) : QWidget(parent) {
	QFileSystemModel * model = new QFileSystemModel(this);
	model->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
	model->setRootPath("/");

	_line = new QLineEdit(this);

	_tree = new QTreeView(this);
	_tree->setModel(model);
	_tree->header()->setVisible(false);

	for (int i = 1; i < _tree->header()->count(); i++)
		_tree->header()->hideSection(i);

	QVBoxLayout * layout = new QVBoxLayout;
	layout->addWidget(_line);
	layout->addWidget(_tree);

	layout->setContentsMargins(2, -1, 2, -1);

	setLayout(layout);

	connect(_tree, SIGNAL(clicked(const QModelIndex &)), this, SLOT(indexChanged(const QModelIndex &)));
	connect(_line, SIGNAL(returnPressed()), this, SLOT(typedPathChanged()));
}

QString DirectorySelectorWidget::path() const {
	return qobject_cast<QFileSystemModel *>(_tree->model())->filePath(_tree->currentIndex());
}

void DirectorySelectorWidget::setPath(const QString & path) {
	const QModelIndex i = qobject_cast<QFileSystemModel *>(_tree->model())->index(path, 0);

	_tree->setCurrentIndex(i);
	_tree->expand(i);
	_line->setText(path);
}

void DirectorySelectorWidget::indexChanged(const QModelIndex & idx) {
	const QString path = qobject_cast<QFileSystemModel *>(_tree->model())->filePath(idx);
	_line->setText(path);

	emit pathSelected(path);
}

void DirectorySelectorWidget::typedPathChanged() {
	const QString path = _line->text();
	const QModelIndex idx = qobject_cast<QFileSystemModel *>(_tree->model())->index(path);

	_tree->setCurrentIndex(idx);
	_tree->expand(idx);

	emit pathSelected(path);
}
