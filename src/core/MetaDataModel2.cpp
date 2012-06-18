
#include <QCoreApplication>
#include <QFileInfo>
#include <QMimeData>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QThread>
#include <QUrl>

#include "MediaScanner.h"
#include "MetaDataModel2.h"
#include "MetaDataWriter.h"

#include <QDebug>

volatile bool abortAction = false;

MetaDataModel2::MetaDataModel2(QObject * parent)
: QAbstractItemModel(parent), _networkManager(0), _recursive(false), _locked(false),
  _editInline(false) {

	_fields.insert(MetaData::TitleField, tr("Title"));
	_fields.insert(MetaData::ArtistField, tr("Artist"));
	_fields.insert(MetaData::AlbumField, tr("Album"));
	_fields.insert(MetaData::LengthField, tr("Length"));
	_fields.insert(MetaData::YearField, tr("Year"));
	_fields.insert(MetaData::DiscNumberField, tr("Disc"));
	_fields.insert(MetaData::NumberField, tr("#"));
	_fields.insert(MetaData::MaxNumberField, tr("Total"));
	_fields.insert(MetaData::GenreField, tr("Genre"));
	_fields.insert(MetaData::CommentField, tr("Comment"));
	_fields.insert(MetaData::OriginalArtistField, tr("Original"));
	_fields.insert(MetaData::ComposerField, tr("Composer"));
	_fields.insert(MetaData::UrlField, tr("URL"));
	_fields.insert(MetaData::EncoderField, tr("Encoder"));
	_fields.insert(MetaData::PathField, tr("Path"));
	_fields.insert(MetaData::PicturesField, tr("Pictures"));

	qRegisterMetaType<MetaData>("MetaData");

	connect(this, SIGNAL(actionStarted()), SLOT(lock()));
	connect(this, SIGNAL(actionFinished()), SLOT(unlock()));
}

void MetaDataModel2::setNetworkManager(QNetworkAccessManager * manager) {
	_networkManager = manager;
}

MetaData MetaDataModel2::metaData(int row) const {
	return _data.value(row);
}

bool MetaDataModel2::addImage(int row, const MetaDataImage & image) {
	if (image.null() || row < 0 || row >= rowCount())
		return false;

	const QModelIndex idx = index(row, MetaData::PicturesField);

	beginInsertRows(idx, rowCount(idx), rowCount(idx));

	backup(row);

	_data[row].addImage(image);

	endInsertRows();

	qDebug() << "Image inserted" << _data[row].imageCount();

	return true;
}

bool MetaDataModel2::addImage(const QModelIndex & idx, const MetaDataImage & image) {
	return addImage(idx.row(), image);
}

void MetaDataModel2::setImages(const QModelIndex & idx, const QList<MetaDataImage> & images) {
	if (idx.model() != this) {
		qWarning() << "MetaDataModel2::setImages(): Invalid model!";

		qDebug() << "----------";
		qDebug() << idx.model();
		qDebug() << this;
		qDebug() << "----------";
		return;
	}
	
	const QModelIndex real = idx.sibling(idx.row(), MetaData::PicturesField);
	
	removeRows(0, rowCount(real), real);

	foreach (const MetaDataImage image, images) {
		addImage(idx.row(), image);
	}
}

QVariant MetaDataModel2::headerData(int section, Qt::Orientation orientation, int role) const {
	if (orientation != Qt::Horizontal)
		return QVariant();
	
	if (role != Qt::DisplayRole)
		return QVariant();

	return _fields.value(section);
}

QVariant MetaDataModel2::data(const QModelIndex & idx, int role) const {
	if (!idx.isValid() || idx.model() != this)
		return QVariant();

	const int r = idx.row();
	const int c = idx.column();

	if (idx.parent().isValid()) {
		
		switch (role) {
			case Qt::DisplayRole:
				return _data.at(idx.parent().row()).image(r).description();

			case Qt::DecorationRole:
				return _data.at(idx.parent().row()).image(r).small();

			case MetaDataImage::MetaTypeRole:
				return _data.at(idx.parent().row()).image(r).type();

			default:
				return QVariant();
		}
	}

	switch (role) {
		case Qt::EditRole:
			return _data.at(r).get((MetaData::Field)c);
			
		case Qt::DisplayRole: {
			const QVariant value = _data.at(r).get((MetaData::Field)c);

			switch (c) {
				case MetaData::PathField:
					return QFileInfo(value.toString()).fileName();

				case MetaData::PicturesField:
					return value.toList().count();

				default:
					return _data.at(r).get((MetaData::Field)c);
			}
		}

		case Qt::ForegroundRole:
			if (!_original.at(r).null())
				return Qt::red;
			
			return QVariant();

		case FieldTypeRole:	
			switch (c) {
				case MetaData::DiscNumberField:
				case MetaData::LengthField:
				case MetaData::MaxNumberField:
				case MetaData::NumberField:
				case MetaData::YearField:
					return (int)QVariant::Int;

				case MetaData::PicturesField:
					return (int)QVariant::UserType;

				default:
					return (int)QVariant::String;
			}

		case FieldChangedRole: {
			MetaData::Field mr = MetaData::Field(r);
			MetaData::Field mc = MetaData::Field(c);

			if (_original.at(mr).null()) {
				return false;
			}
			
			return _original.at(mr).get(mc).toString() != _data.at(mr).get(mc).toString();
		}

		default:
			return QVariant();
	}
}

bool MetaDataModel2::setData(const QModelIndex & idx, const QVariant & value, int role) {
	if (!idx.isValid() || idx.model() != this)
		return false;

	if (idx.column() == MetaData::PicturesField && !idx.parent().isValid())
		return false;

	if (idx.parent().isValid()) {
		const int pRow = idx.parent().row();
		const int row = idx.row();
	
		QList<MetaDataImage> images = _data.at(pRow).images();
		
		switch (role) {
			// Changing the QImage is not supported
			// Instead have to remove a row and insert a new one via addImage().
			
			case Qt::DisplayRole:
				images[row].setDescription(value.toString());
				break;

			case MetaDataImage::MetaTypeRole:
				images[row].setType(value.toInt());
				break;

			default:
				return false;
		}

		backup(pRow);
		_data[pRow].setImages(images);

		emit dataChanged(idx, idx);

		return true;
	}

	if (role != Qt::EditRole)
		return false;

	const int r = idx.row();
	MetaData::Field c = (MetaData::Field)idx.column();

	if (_data.at(r).get(c).toString() == value.toString())
		return true;
	
	// This rule also filters out the situation when existing value is null and the new
	// value is 0.
	if (idx.data(FieldTypeRole).toInt() == QVariant::Int && idx.data().toInt() == value.toInt())
		return true;

	backup(r);

	_data[r].set((MetaData::Field)idx.column(), value);

	emit dataChanged(idx, idx);

	return true;
}

bool MetaDataModel2::removeRows(int start, int count, const QModelIndex & parent) {
	if (start < 0 || count < 1)
		return false;

	const int pRow = parent.row();

	beginRemoveRows(parent, start, start+count-1);

	while (count-- > 0) {
		if (parent.isValid()) {
			backup(pRow);

			_data[pRow].removeImage(start);
		} else {
			_data.removeAt(start);
			_original.removeAt(start);
		}
	}

	endRemoveRows();

	return true;
}

int MetaDataModel2::columnCount(const QModelIndex &) const {
	return MetaData::PicturesField + 1;
}

int MetaDataModel2::rowCount(const QModelIndex & parent) const {
	if (parent.isValid())
		return _data.at(parent.row()).imageCount();

	return _data.count();
}

QModelIndex MetaDataModel2::index(int row, int column, const QModelIndex & parent) const {
	if (parent.isValid())
		return createIndex(row, column, parent.row() * 10000 + parent.column());
	
	return createIndex(row, column, 0);
}

QModelIndex MetaDataModel2::parent(const QModelIndex & idx) const {
	qint16 id = idx.internalId();
	
	if (id <= 0)
		return QModelIndex();

	int row = id / 10000;
	int column = id % 100;

	return index(row, column);
}

Qt::ItemFlags MetaDataModel2::flags(const QModelIndex & idx) const {

	Qt::ItemFlags flags = QAbstractItemModel::flags(idx);

	// Allow drops on invalid indices too so that we can drop items on empty
	// views aswell.
	if (!idx.parent().isValid() || idx.column() == MetaData::PicturesField) {
		flags |= Qt::ItemIsDropEnabled;
	}

	if (_editInline && !idx.parent().isValid() && idx.column() != MetaData::PicturesField) {
		flags |= Qt::ItemIsEditable;
	}

	return flags;
}

QStringList MetaDataModel2::mimeTypes() const {	
	return QStringList() << "text/uri-list" << "image/png" << "image/jpeg"
		<< "image/gif";
}

bool MetaDataModel2::dropMimeData(const QMimeData * data, Qt::DropAction,
	int, int, const QModelIndex & parent) {

	// Should later add support for dropping a directory!

	if (parent.isValid() && parent.column() == MetaData::PicturesField) {
		if (data->hasUrls()) {
			foreach (const QUrl url, data->urls()) {

				QImage image;
				
				if (url.scheme() == "file") {
					image = QImage(url.path());

					if (image.isNull()) {
						emit actionError(tr("Invalid image"));
					}
				} else {
					image = downloadImage(url);
				}

				if (image.isNull()) {
					return false;
				}

				const QString description = QFileInfo(url.path()).baseName();

				addImage(parent, MetaDataImage(image, 0, description));
			}

			return true;
		} else if (data->hasImage()) {
			const QImage image = qvariant_cast<QImage>(data->imageData());

			addImage(parent, MetaDataImage(image));

			return true;
		}
	}
	

	return false;
}




/**
 * PUBLIC SLOTS
 **/

void MetaDataModel2::add(MetaData data) {
	beginInsertRows(QModelIndex(), rowCount(), rowCount());

	_data << data;
	_original << MetaData();

	endInsertRows();
}

void MetaDataModel2::readDirectory(const QString & dir) {
	if (dir.isEmpty())
		return;

	setDirectory(dir);

	if (_locked) {
		qDebug() << "Cannot run scanner because model is locked.";
		return;
	}

	::abortAction = false;

	clear();

	emit metaDataStateChanged(false);

	MediaScanner * scanner = new MediaScanner();

	scanner->setRecursive(recursive());
	scanner->setPath(dir);

	QThread * thread = new QThread(this);
	scanner->moveToThread(thread);

	connect(scanner, SIGNAL(itemFound(MetaData)), SLOT(add(MetaData)));

	connect(scanner, SIGNAL(finished()), SIGNAL(actionFinished()));
	connect(scanner, SIGNAL(started()), SIGNAL(actionStarted()));
	connect(scanner, SIGNAL(progress(int)), SIGNAL(actionProgress(int)));
	connect(scanner, SIGNAL(maximumChanged(int)), SIGNAL(actionMaximumChanged(int)));

	connect(scanner, SIGNAL(finished()), scanner, SLOT(deleteLater()));
	connect(scanner, SIGNAL(finished()), thread, SLOT(quit()));
	connect(scanner, SIGNAL(finished()), thread, SLOT(deleteLater()));

	connect(thread, SIGNAL(started()), scanner, SLOT(scan()));

	thread->start();
}

void MetaDataModel2::reload() {
	readDirectory(_directory);
}

void MetaDataModel2::restore() {
	
}

void MetaDataModel2::save() {
	if (_locked) {
		qWarning() << "Cannot run writer because model is locked!";
		return;
	}

	::abortAction = false;

	QMap<QString, MetaData> changed;

	for (int i = 0; i < rowCount(); i++) {
		if (_original.at(i).null())
			continue;

		changed.insert(_original.at(i).get(MetaData::PathField).toString(), _data.at(i));
		_original.replace(i, MetaData());
	}

	emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
	emit metaDataStateChanged(false);

	QThread * thread = new QThread(this);
	
	MetaDataWriter * writer = new MetaDataWriter();
	writer->moveToThread(thread);
	writer->setQueue(changed);

	connect(writer, SIGNAL(started()), SIGNAL(actionStarted()));
	connect(writer, SIGNAL(finished()), SIGNAL(actionFinished()));
	connect(writer, SIGNAL(progress(int)), SIGNAL(actionProgress(int)));
	connect(writer, SIGNAL(maximumChanged(int)), SIGNAL(actionMaximumChanged(int)));

	connect(writer, SIGNAL(finished()), writer, SLOT(deleteLater()));
	connect(writer, SIGNAL(finished()), thread, SLOT(quit()));
	connect(writer, SIGNAL(finished()), thread, SLOT(deleteLater()));

	connect(thread, SIGNAL(started()), writer, SLOT(write()));
	
	thread->start();
}

void MetaDataModel2::backup(int row) {
	if (row < 0 || row >= rowCount())
		return;

	if (!_original.at(row).null())
		return;

	_original.replace(row, _data.at(row));

	emit metaDataStateChanged(true);
}

QImage MetaDataModel2::downloadImage(const QUrl & url) {
	QNetworkReply * reply = _networkManager->get(QNetworkRequest(url));

	emit actionStarted();

	connect(reply, SIGNAL(finished()), SIGNAL(actionFinished()));
	
	connect(reply, SIGNAL(downloadProgress(qint64, qint64)),
		SLOT(networkActionProgressChanged(qint64, qint64)));

	while (!reply->isFinished()) {
		QCoreApplication::processEvents(QEventLoop::AllEvents, 500);

		if (::abortAction) {
			reply->abort();
			return QImage();
		}
	}

	const QImage image = QImage::fromData(reply->readAll());

	reply->deleteLater();

	if (reply->error() != QNetworkReply::NoError) {
		emit actionError(tr("Network error (code %1)").arg(reply->error()));
	}

	if (image.isNull()) {
		emit actionError(tr("Invalid image"));
	}

	return image;
}
