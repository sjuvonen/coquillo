#ifndef BOOKMARKMODEL_H
#define BOOKMARKMODEL_H

#include <QSortFilterProxyModel>

class QStandardItemModel;

class BookmarkModel : public QSortFilterProxyModel {
	Q_OBJECT

	public:
		enum Roles { FilePathRole = Qt::UserRole+1, DisplayNameRole=Qt::UserRole+2 };
		
		BookmarkModel(QObject * parent=0);

		bool setName(const QString & name, const QString & path);
		bool setName(const QString & name, int row);

		QModelIndex indexForPath(const QString & path) const;
		
		QVariant data(const QModelIndex &, int role=Qt::DisplayRole) const;
		bool setData(const QModelIndex &, const QVariant &, int role = Qt::EditRole);

	public slots:
		bool addPath(const QString & path, const QString & name=QString());
		bool removePath(const QString &);

	private slots:
		void saveBookmarks();

	private:
		void loadBookmarks();

		QStandardItemModel * _source;
};

#endif