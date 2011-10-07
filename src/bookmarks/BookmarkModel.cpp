
#include <QFileInfo>
#include <QSettings>

#include "BookmarkModel.h"

#include <QDebug>

BookmarkModel::BookmarkModel(QObject * parent) : QStandardItemModel(parent) {

	insertColumn(0);

	loadBookmarks();

	connect(this, SIGNAL(rowsRemoved(QModelIndex, int, int)),
		SLOT(saveBookmarks()));
}

#include <QStringList>

bool BookmarkModel::addPath(const QString & path, const QString & name) {
	QFileInfo info(path);
	const QString absolutePath = info.absoluteFilePath();

	if (!info.exists())
		return false;

	if (indexForPath(absolutePath).isValid())
		return false;

	QStandardItem * item = new QStandardItem;

	item->setData(absolutePath, FilePathRole);

	if (!name.isEmpty())
		item->setData(name, DisplayNameRole);

	appendRow(item);
	saveBookmarks();
}

bool BookmarkModel::removePath(const QString & path) {

	bool ok = removeRow( indexForPath(path).row() );

	if (ok)
		saveBookmarks();
	
	return ok;
}

bool BookmarkModel::setName(const QString & name, const QString & path) {
	return setName( name, indexForPath(path).row() );
}

bool BookmarkModel::setName(const QString & name, int row) {
	if (row == -1)
		return false;

	setData(index(row, 0), name, DisplayNameRole);
}

QVariant BookmarkModel::data(const QModelIndex & idx, int role) const {
	if (!idx.isValid())
		return QVariant();

	switch (role) {
		case Qt::DisplayRole:
		case Qt::EditRole:

			if (!QStandardItemModel::data(idx, DisplayNameRole).isNull())
				role = DisplayNameRole;
			else
				role = FilePathRole;

		default:
			return QStandardItemModel::data(idx, role);
	}
}

bool BookmarkModel::setData(const QModelIndex & idx, const QVariant & value, int role) {

	if (role == Qt::EditRole)
		role = FilePathRole;
	
	bool ok = QStandardItemModel::setData(idx, value, role);

	if (ok)
		saveBookmarks();

	return ok;
}

QModelIndex BookmarkModel::indexForPath(const QString & path) const {
	return match(index(0, 0), FilePathRole, path, 1, Qt::MatchExactly).value(0);
}





void BookmarkModel::loadBookmarks() {
	QSettings settings;

	int size = settings.beginReadArray("Bookmarks");

	for (int i = 0; i < size; i++) {
		settings.setArrayIndex(i);

		QStandardItem * item = new QStandardItem;
		item->setData(settings.value("Path"), FilePathRole);
		item->setData(settings.value("DisplayName"), DisplayNameRole);

		appendRow(item);
	}

	settings.endArray();
}


void BookmarkModel::saveBookmarks() {

	QSettings settings;

	settings.remove("Bookmarks");
	settings.beginWriteArray("Bookmarks");

	for (int i = 0; i < rowCount(); i++) {
		const QModelIndex idx = index(i, 0);
		settings.setArrayIndex(i);
		settings.setValue("Path", idx.data(FilePathRole));
		settings.setValue("DisplayName", idx.data(DisplayNameRole));
	}

	settings.endArray();
}
