
#include <QDebug>
#include <QMetaType>
#include <QSettings>
#include <QThread>

#include "MediaScanner.h"
#include "MetaDataModel.h"
#include "MetaDataWriter.h"

/**
 * When turned on, the ongoing action (media scan, writing tags) will be aborted
 **/
volatile bool abortAction = false;

MetaDataModel::MetaDataModel(QObject * parent)
: QStandardItemModel(parent), _slave(0), _recursive(false), _locked(false) {

	_fieldNames.insert(MetaData::TitleField, tr("Title"));
	_fieldNames.insert(MetaData::ArtistField, tr("Artist"));
	_fieldNames.insert(MetaData::AlbumField, tr("Album"));
	_fieldNames.insert(MetaData::LengthField, tr("Length"));
	_fieldNames.insert(MetaData::YearField, tr("Year"));
	_fieldNames.insert(MetaData::DiscNumberField, tr("Disc"));
	_fieldNames.insert(MetaData::NumberField, tr("#"));
	_fieldNames.insert(MetaData::MaxNumberField, tr("Total"));
	_fieldNames.insert(MetaData::GenreField, tr("Genre"));
	_fieldNames.insert(MetaData::CommentField, tr("Comment"));
	_fieldNames.insert(MetaData::OriginalArtistField, tr("Original"));
	_fieldNames.insert(MetaData::ComposerField, tr("Composer"));
	_fieldNames.insert(MetaData::UrlField, tr("URL"));
	_fieldNames.insert(MetaData::EncoderField, tr("Encoder"));
	_fieldNames.insert(MetaData::PathField, tr("Path"));
	_fieldNames.insert(MetaData::PicturesField, tr("Pictures"));

	qRegisterMetaType<MetaData>("MetaData");

	insertColumns(0, 16);
	setHorizontalHeaderLabels(_fieldNames.values());

	_slave = new QThread(this);

	connect(_slave, SIGNAL(started()), SLOT(lock()));
	connect(_slave, SIGNAL(finished()), SLOT(unlock()));
}

MetaData MetaDataModel::metaData(const QModelIndex & idx) const {
	if (!idx.isValid())
		return MetaData();

	return idx.data(MetaDataRole).value<MetaData>();
}

MetaData MetaDataModel::metaData(int row) const {
	return metaData(index(row, 0));
}

QList<MetaData> MetaDataModel::all() const {
	QList<MetaData> list;

	for (int i = 0; i < rowCount(); i++)
		list << metaData(i);

	return list;
}

QVariant MetaDataModel::data(const QModelIndex & idx, int role) const {
	const int col = idx.column();

	switch (role) {
		case Qt::BackgroundRole: {
			if (false && QSettings().value("SubTreeDepth").toBool() == false)
				return QStandardItemModel::data(idx, role);

			const QString path = idx.sibling(idx.row(), MetaData::PathField).data().toString();

			if (path.count('/') == 0)
				return QStandardItemModel::data(idx, role);

			static QColor c = QColor(Qt::green).lighter(180);
			c.setAlpha(50);

			return c.darker( 100 + 20 * path.count('/') );
		}

		case Qt::DisplayRole:
			if (col == MetaData::PathField) {
				const QVariant v = QStandardItemModel::data(idx, role);
				const QRegExp rx( QString("^%1/?").arg( QRegExp::escape(_directory)) );

				if (!v.isNull())
					return v.toString().remove(rx);
			} else if (col == MetaData::PicturesField) {

				return QString("%1 pics").arg(rowCount(idx));

				if (!idx.parent().isValid())
					return QString(tr("%1 pictures")).arg(rowCount(idx));

				return "-";
			}

			break;

		case Qt::ForegroundRole:
			if (!idx.sibling(idx.row(), 0).data(ChangedStatusRole).isNull())
				return Qt::red;

			break;

		case Qt::TextAlignmentRole:
			switch (idx.column()) {
				case MetaData::NumberField:
				case MetaData::PicturesField:
					return Qt::AlignCenter;

				default:
					;
			}

			break;

		default:
			;
	}
	
	return QStandardItemModel::data(idx, role);
}

bool MetaDataModel::setData(const QModelIndex & idx, const QVariant & value, int role) {

	if (idx.model() != this) {
		qDebug() << "MetaDataModel: index from wrong model given to setData()";
		return false;
	}

	// Pictures are stored in child indices in the MetaData::PicturesField column
	if (idx.parent().isValid()) {
		if (role != OriginalDataRole && idx.data(OriginalDataRole).isNull() && !idx.data().isNull()) {
			
			if (idx.column() != MetaDataImage::ImageField && value.toString() != idx.data().toString()) {
				QStandardItemModel::setData(idx, idx.data(), OriginalDataRole);
				QStandardItemModel::setData(idx.parent().sibling(idx.parent().row(), 0), true, ChangedStatusRole);

				emit metaDataStateChanged(true);
			}
		}

		QStandardItemModel::setData(idx, value, role);
	}
	
	if (role == Qt::EditRole) {
		// Prevent overwriting a value with itself to avoid messing up the status
		// of modified metadata
		if (idx.column() != MetaData::PicturesField && idx.data(role).toString() == value.toString())
			return false;

		// EditorWidget sets numeric fields as 0 but we do not want to display
		// a numeric zero when there is no original value (i.e. it's an empty string
		// in the model).
		if (role == Qt::EditRole) {
			switch (idx.column()) {
				case MetaData::LengthField:
				case MetaData::YearField:
				case MetaData::DiscNumberField:
				case MetaData::NumberField:
				case MetaData::MaxNumberField:
					if (value.toInt() == 0 && !idx.data(role).isNull() && idx.data(role).toString().isEmpty())
						return false;

				default:
					;
			}
		}

		// Copy the current value to OriginalDataRole to enable reverting changes.
		// Only the first non null value will be copied to OriginalDataRole.
		if (!idx.data().isNull() && idx.data(OriginalDataRole).isNull()) {
			qDebug();
			qDebug() << "*** MetaDataModel ***";
			qDebug() << "Old:" << idx.data(Qt::EditRole).toString();
			qDebug() << "New:" << value.toString();
			qDebug();

			QStandardItemModel::setData(idx, idx.data(Qt::EditRole), OriginalDataRole);
			QStandardItemModel::setData(idx.sibling(idx.row(), 0), true, ChangedStatusRole);

			emit metaDataStateChanged(true);
		}
	}

	return QStandardItemModel::setData(idx, value, role);
}





bool MetaDataModel::add(const MetaData & data) {
	int row = rowCount();
	
	if (!insertRow(row))
		return false;

	blockSignals(true);

	const QModelIndex idx = index(row, MetaData::PicturesField);

	insertColumns(0, 3, idx);

	if (data.imageCount()) {
		insertRows(0, data.imageCount(), idx);

		for (int i = 0; i < data.imageCount(); i++) {
			const MetaDataImage image = data.image(i);

			setData(index(i, 0, idx), QVariant::fromValue<QImage>(image.image()));
			setData(index(i, 1, idx), image.description());
			setData(index(i, 2, idx), image.type());
			
			setData(index(i, 0, idx), true, OriginalImageRole);
		}
	}

	for (int i = 0; i < columnCount(); i++) {
		if (i == MetaData::PicturesField)
			continue;

		QVariant value = data.get((MetaData::Field)i);

		if (value.isNull())
			value = QString();
		
		setData(index(row, i), value);
	}

	blockSignals(false);

	emit dataChanged(idx.sibling(idx.row(), 0), idx.sibling(idx.row(), columnCount(idx)-1));

	return true;
}

void MetaDataModel::readDirectory(const QString & dir) {
	if (dir.isEmpty())
		return;
	
	setDirectory(dir);

	if (_locked) {
		qDebug() << "Cannot run scanner because model is locked.";
		return;
	}

	::abortAction = false;

	removeRows(0, rowCount());

	emit metaDataStateChanged(false);

	MediaScanner * scanner = new MediaScanner();

	scanner->setRecursive(recursive());
	scanner->setPath(dir);

	connect(scanner, SIGNAL(itemFound(MetaData)), SLOT(add(MetaData)));

	connect(scanner, SIGNAL(finished()), SIGNAL(actionFinished()));
	connect(scanner, SIGNAL(started()), SIGNAL(actionStarted()));
	connect(scanner, SIGNAL(progress(int)), SIGNAL(actionProgress(int)));
	connect(scanner, SIGNAL(maximumChanged(int)), SIGNAL(actionMaximumChanged(int)));

	connect(scanner, SIGNAL(finished()), scanner, SLOT(deleteLater()));
	connect(scanner, SIGNAL(finished()), _slave, SLOT(quit()));

	connect(_slave, SIGNAL(started()), scanner, SLOT(scan()));

	scanner->moveToThread(_slave);
	_slave->start();
}

void MetaDataModel::reload() {
	readDirectory(_directory);
}

void MetaDataModel::save() {
	if (_locked) {
		qDebug() << "Cannot run writer because model is locked.";
		return;
	}

	::abortAction = false;

	QMap<QString, MetaData> changed;

	for (int i = 0; i < rowCount(); i++) {
		if (index(i, 0).data(ChangedStatusRole).isNull())
			continue;

		MetaData metaData;
		QString path;
		
		for (int j = 0; j < columnCount(); j++) {
			const QModelIndex idx = index(i, j);

			if (!idx.data(OriginalDataRole).isNull())
				metaData.set((MetaData::Field)j, idx.data(Qt::EditRole));
		}

		const QModelIndex picsIdx = index(i, MetaData::PicturesField);

		for (int j = 0; j < rowCount(picsIdx); j++) {
			MetaDataImage img;
			img.setImage(index(j, MetaDataImage::ImageField, picsIdx).data().value<QImage>());
			img.setDescription(index(j, MetaDataImage::DescriptionField, picsIdx).data().toString());
			img.setType(index(j, MetaDataImage::TypeField, picsIdx).data().toInt());

			metaData.addImage(img);
		}

		removeRows(0, rowCount(picsIdx), picsIdx);

		if (metaData.null())
			continue;

		if (metaData.has(MetaData::MaxNumberField))
			metaData.set(MetaData::NumberField, index(i, MetaData::NumberField).data());

		int role = metaData.has(MetaData::PathField) ? (int)OriginalDataRole : (int)Qt::EditRole;

		changed.insert(index(i, MetaData::PathField).data(role).toString(), metaData);
	}

	blockSignals(true);

	for (int i = 0; i < rowCount(); i++) {
		for (int j = 0; j < columnCount(); j++)
			setData(index(i, j), QVariant(), OriginalDataRole);

		setData(index(i, 0), QVariant(), ChangedStatusRole);
	}

	blockSignals(false);

	emit metaDataStateChanged(false);
	emit dataChanged(index(0, 0), index(rowCount()-1, columnCount()-1));

	MetaDataWriter * writer = new MetaDataWriter();
	writer->setQueue(changed);

	connect(writer, SIGNAL(started()), SIGNAL(actionStarted()));
	connect(writer, SIGNAL(finished()), SIGNAL(actionFinished()));
	connect(writer, SIGNAL(progress(int)), SIGNAL(actionProgress(int)));
	connect(writer, SIGNAL(maximumChanged(int)), SIGNAL(actionMaximumChanged(int)));

	connect(writer, SIGNAL(finished()), writer, SLOT(deleteLater()));
	connect(writer, SIGNAL(finished()), _slave, SLOT(quit()));

	connect(_slave, SIGNAL(started()), writer, SLOT(write()));

	writer->moveToThread(_slave);
	_slave->start();
}

void MetaDataModel::restore() {
	blockSignals(true);
	
	for (int i = 0; i < rowCount(); i++) {
		for (int j = 0; j < columnCount(); j++) {
			const QVariant original = index(i, j).data(OriginalDataRole);

			if (j == MetaData::PicturesField) {
				const QModelIndex picsIdx = index(i, j);

				for (int k = rowCount(picsIdx) - 1; k >= 0; k--) {

					if (picsIdx.child(k, 0).data(OriginalImageRole).isNull()) {
						removeRow(k, picsIdx);
						continue;
					}

					for (int n = 0; n < columnCount(picsIdx); n++) {
						const QModelIndex cidx = picsIdx.child(k, n);

						if (!cidx.data(OriginalDataRole).isNull()) {
							setData(cidx, cidx.data(OriginalDataRole));
							setData(cidx, QVariant(), OriginalDataRole);
						}
					}
					
				}
		
				
			} else if (!original.isNull()) {
				setData(index(i, j), original);
				setData(index(i, j), QVariant(), OriginalDataRole);
			}
		}
		
		setData(index(i, 0), QVariant(), ChangedStatusRole);
	}

	blockSignals(false);

	emit metaDataStateChanged(false);
	emit dataChanged(index(0, 0), index(rowCount()-1, columnCount()-1));
}

 