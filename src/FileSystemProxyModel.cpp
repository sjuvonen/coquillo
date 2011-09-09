
#include <QFileSystemModel>

#include "FileSystemProxyModel.h"

FileSystemProxyModel::FileSystemProxyModel(QObject * parent)
: QSortFilterProxyModel(parent) {


}

QString FileSystemProxyModel::rootPath() const {
	if (sourceModel())
		return sourceModel()->rootPath();

	return QString();
}

void FileSystemProxyModel::setRootPath(const QString & path) {
	if (sourceModel())
		sourceModel()->setRootPath(path);
}

QFileSystemModel * FileSystemProxyModel::sourceModel() const {
	return qobject_cast<QFileSystemModel*>(QSortFilterProxyModel::sourceModel());
}

void FileSystemProxyModel::setSourceModel(QFileSystemModel * m) {
	QSortFilterProxyModel::setSourceModel(m);
}

QVariant FileSystemProxyModel::data(const QModelIndex & idx, int role) const {
	if (role == Qt::SizeHintRole) {
		return QSize(100, 26);
	}

	return QSortFilterProxyModel::data(idx, role);

}
