
#include <QFileInfo>
#include <QFileSystemModel>
#include <QHeaderView>
#include <QInputDialog>
#include <QLineEdit>
#include <QMenu>
#include <QModelIndex>
#include <QMouseEvent>
#include <QToolButton>
#include <QTreeView>
#include <QVBoxLayout>

#include "DirectorySelectorWidget.h"
#include "FileSystemProxyModel.h"

#include "ui_DirectorySelector.h"

#include <QDebug>

DirectorySelectorWidget::DirectorySelectorWidget(QWidget * parent)
: QWidget(parent) {
	_model = new QFileSystemModel(this);
	_model->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
	_model->setReadOnly(false);

	_proxy = new FileSystemProxyModel(this);
	_proxy->setSourceModel(_model);
// 	_proxy->setDynamicSortFilter(true);

	_ui = new Ui::DirectorySelector;
	_ui->setupUi(this);

	_ui->directories->setModel(_proxy);
	_ui->directories->viewport()->installEventFilter(this);

	#if QT_MINOR_VERSION >= 7 || QT_MAJOR_VERSION > 4
	_ui->location->setPlaceholderText(tr("Path"));
	#endif
}

QString DirectorySelectorWidget::path() const {
	QString path = _model->filePath(_ui->directories->currentIndex());
	
	return path.isEmpty() ? _ui->location->text() : path;
}

bool DirectorySelectorWidget::eventFilter(QObject * object, QEvent * event) {
// 	return false;
	
	if (object == _ui->directories->viewport() && event->type() == QEvent::MouseButtonPress) {
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





/* PUBLIC SLOTS */

void DirectorySelectorWidget::setPath(const QString & path) {
	const QString curPath = _ui->location->text();
	
	if (curPath == path)
		return;
	
	const QModelIndex srcIdx = _model->index(path);
	const QModelIndex idx = _proxy->mapFromSource(srcIdx);

	if (idx == _ui->directories->currentIndex())
		return;

	if (path.indexOf(curPath) != 0)
		setRootPath(path);

	_ui->directories->setCurrentIndex(idx);
	_ui->directories->expand(idx);
	_ui->location->setText(path);
}

void DirectorySelectorWidget::setRootPath(const QString & path) {
	
	_model->setRootPath(path);

	const QModelIndex idx = _proxy->mapFromSource( _model->index(path) );
	_ui->directories->setRootIndex(idx);

	if (_ui->location->text().isEmpty() || _ui->location->text().indexOf(path) != 0) {
		_ui->location->setText(path);
		emit pathChanged(path);
	}
}

void DirectorySelectorWidget::setRootIndex(const QModelIndex & idx) {
	if (!idx.isValid())
		return;

	const QString path = idx.data(QFileSystemModel::FilePathRole).toString();
	
	if (QDir(path).entryList(QStringList(), QDir::Dirs | QDir::NoDotAndDotDot).isEmpty())
		return;
	
	setRootPath( path );
}

void DirectorySelectorWidget::changePath(const QModelIndex & idx) {
	const QString path = _model->filePath( _proxy->mapToSource(idx) );

	if (path == _ui->location->text()) {
		return;
	}
	
	_ui->location->setText(path);

	emit pathChanged(path);
}

void DirectorySelectorWidget::changeToTypedPath() {
	const QString path = _ui->location->text();

	if (!path.isEmpty()) {
		setRootPath(path);
		setPath(path);
	}
}

void DirectorySelectorWidget::goDirectoryUp() {
	const QModelIndex idx = _model->index( _model->rootPath() ).parent();

	if (idx.isValid()) {
		setRootIndex( _proxy->mapFromSource(idx) );
		_ui->location->setText(_model->filePath(idx));
	}
}





void DirectorySelectorWidget::showTreeContextMenu(const QPoint & point) {
	QMenu menu(this);
	
	QAction * setRoot = menu.addAction(tr("Set as root"));
	QAction * addBookmark = menu.addAction(tr("Add to bookmarks"));
	QAction * renameDir = menu.addAction(tr("Rename..."));

	QModelIndex idx = _ui->directories->indexAt(point);

	if (idx.isValid()) {
		const QString path = idx.data(QFileSystemModel::FilePathRole).toString();

		if (QDir(path).entryList(QStringList(), QDir::Dirs | QDir::NoDotAndDotDot).isEmpty())
			setRoot->setDisabled(true);
	}

	QAction * selected = menu.exec( _ui->directories->mapToGlobal(point + QPoint(5, 5)) );

	if (selected == setRoot) {
		setRootIndex( _ui->directories->currentIndex() );
	} else if (selected == addBookmark) {
		QString path = _ui->directories->currentIndex().data(
			QFileSystemModel::FilePathRole
		).toString();

		if (!path.isEmpty())
			emit bookmarked(path);
	} else if (selected == renameDir) {
		_ui->directories->edit(_ui->directories->currentIndex());
	}
}
