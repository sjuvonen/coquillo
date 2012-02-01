#ifndef FILESYSTEMPROXYMODEL_H
#define FILESYSTEMPROXYMODEL_H

#include <QSortFilterProxyModel>

class QFileSystemModel;

class FileSystemProxyModel : public QSortFilterProxyModel {
	Q_OBJECT

	public:
		FileSystemProxyModel(QObject * parent=0);

		QString rootPath() const;
		void setRootPath(const QString &);

		QFileSystemModel * sourceModel() const;
		void setSourceModel(QFileSystemModel *);

		int columnCount(const QModelIndex & =QModelIndex()) const {
			return 1;
		}

		QVariant data(const QModelIndex & idx, int role=Qt::DisplayRole) const;
};

#endif