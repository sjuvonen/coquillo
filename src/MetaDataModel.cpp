
#include <QThread>
#include <QTimer>

#include "MediaScanner.h"
#include "MetaDataModel.h"

#include <QDebug>

MetaDataModel * MetaDataModel::s_instance = 0;

MetaDataModel * MetaDataModel::instance() {
	if (!s_instance)
		s_instance = new MetaDataModel;

	return s_instance;
}

MetaDataModel::MetaDataModel(QObject * parent)
: QStandardItemModel(parent), _recurse(false) {

	QThread * _utilThread = new QThread;
	
	_scanner = new MediaScanner;
	_scanner->moveToThread(_utilThread);

	_utilThread->start();

	_columnNames.insert(0, "Title");
	_columnNames.insert(1, "Artist");
	_columnNames.insert(2, "Album");
	_columnNames.insert(3, "Year");
	_columnNames.insert(4, "Disc");
	_columnNames.insert(5, "Number");
	_columnNames.insert(6, "MaxNumber");
	_columnNames.insert(7, "Genre");
	_columnNames.insert(8, "Comment");
	_columnNames.insert(9, "OriginalArtist");
	_columnNames.insert(10, "Composer");
	_columnNames.insert(11, "Url");
	_columnNames.insert(12, "Encoder");
	_columnNames.insert(13, "Path");
	_columnNames.insert(14, "Pictures");
	_columnNames.insert(15, "Length");

	QStringList labels = _columnNames.values();
	labels.replace(column("Number"), "#");

	setHorizontalHeaderLabels(labels);

	connect(_scanner, SIGNAL(itemFound(MetaData)),
		SLOT(addItem(MetaData)));
	
}

QVariant MetaDataModel::data(const QModelIndex & idx, int role) const {
	const int row = idx.row();
	const int col = idx.column();
	
	switch (role) {
		case Qt::BackgroundRole: {
			if (!_showDepth)
				return QStandardItemModel::data(idx, role);

			static QColor c(Qt::white);
			c.setAlpha(128);

			return c.darker( 100 + 20 * idx.sibling(idx.row(), column("Path")).data().toString().count('/') );
		}

		case Qt::DisplayRole:
			if (col == column("Path")) {
				const QVariant v = QStandardItemModel::data(idx, role);
				const QRegExp rx( QString("^%1").arg( QRegExp::escape(_directory)) );

				if (!v.isNull())
					return v.toString().remove(rx);
			} else if (col == column("Pictures")) {

				return "n/a";
				
				if (!idx.parent().isValid())
					return QString(tr("%1 pictures")).arg(rowCount(idx));

				return "-";
			}
				return QStandardItemModel::data(idx, role);

		case Qt::ForegroundRole:
			if (!idx.sibling(idx.row(), 0).data(RowModifiedRole).isNull())
				return Qt::red;

		case Qt::TextAlignmentRole:
			if (idx.column() == column("Number"))
				return Qt::AlignCenter;
			else
				return QStandardItemModel::data(idx, role);

		default:
			return QStandardItemModel::data(idx, role);
	}
}

bool MetaDataModel::setData(const QModelIndex & idx, const QVariant & value, int role) {

	if (role == Qt::EditRole && idx.column() != _columnNames.key("Pictures")
		&& idx.data(role).toString() == value.toString())
		return false;

	if (role == Qt::EditRole && !idx.data(Qt::EditRole).isNull() && idx.data(OriginalDataRole).isNull()) {
		QStandardItemModel::setData(idx, idx.data(Qt::EditRole), OriginalDataRole);
		QStandardItemModel::setData(idx.sibling(idx.row(), 0), true, RowModifiedRole);

		emit metaDataStateChanged(true);
	}

	return QStandardItemModel::setData(idx, value, role);
}

int MetaDataModel::pictureCount(const QModelIndex & idx) {
	QStandardItem * parent = item(idx.row(), column("Pictures"));
	return parent ? parent->rowCount() : 0;
}

QList<MetaDataImage> MetaDataModel::pictures(const QModelIndex & idx) const {
	QList<MetaDataImage> pics;

	QStandardItem * parent = item(idx.row(), column("Pictures"));

	for (int i = 0; i < parent->rowCount(); i++)
		pics << parent->child(i)->data(Qt::DisplayRole).value<MetaDataImage>();

	return pics;
}

bool MetaDataModel::addPicture(const QModelIndex & idx, const QVariantMap & data) {
	Q_UNUSED(idx)
	Q_UNUSED(data)

	QStandardItem * parent = item(idx.row(), column("Pictures"));

	if (!parent)
		return false;

	MetaDataImage pic(
		data.value("PICTURE").value<QImage>(),
		data.value("TYPE").toInt(),
		data.value("DESCRIPTION").toString()
	);

	QStandardItem * picitem = new QStandardItem;
	picitem->setData(QVariant::fromValue<MetaDataImage>(pic));

	parent->appendRow(picitem);

	return true;
}

bool MetaDataModel::removePicture(const QModelIndex & idx, int pos) {
	Q_UNUSED(idx)
	Q_UNUSED(pos)

	QStandardItem * parent = item(idx.row(), column("Pictures"));

	if (!parent)
		return false;

	parent->removeRow(pos);

	return true;
}

bool MetaDataModel::editPicture(const QModelIndex & idx, const QVariant & value, int role) {
	return setData(idx, value, role);
}

bool MetaDataModel::editPicture(const QModelIndex & parent, int pos,
	const QVariant & value, int role) {

	const QModelIndex idx = parent.child(pos, 0);

	if (!idx.isValid())
		return false;

	return editPicture(idx, value, role);
}





void MetaDataModel::scan(const QString & dir) {
	if (_directory.isEmpty() && dir.isEmpty())
		return;

	removeRows(0, rowCount());
	_metaData.clear();

	qDebug() << "scan";
	
	if (!dir.isEmpty())
		_directory = dir;

	_scanner->setRecursive(_recurse);
	_scanner->setPath(dir);

	QTimer::singleShot(10, _scanner, SLOT(scan()));
}

void MetaDataModel::saveChanges() {
	
	emit metaDataStateChanged(false);
}

void MetaDataModel::undoChanges() {

	emit metaDataStateChanged(false);
}



/**
 * PRIVATE SLOTS
**/


void MetaDataModel::addItem(const MetaData & data) {
	QList<QStandardItem*> items;

	foreach (int key, Coquillo::fieldNames.keys()) {
		const QString field = Coquillo::fieldNames.value(key);

		QStandardItem * item = new QStandardItem;
		item->setDropEnabled(false);

		if (field != "Pictures") {
			const QVariant value = data.value(field);
			item->setData( value, Qt::EditRole );
		}

		items << item;
	}

	appendRow(items);
	
	
}

