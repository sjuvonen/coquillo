
#include <QErrorMessage>
#include <QSettings>
#include <QStandardItemModel>
#include <QTreeView>

#include "globals.h"

#include "CddbSearchDialog.h"

#include "ui_CddbSearchDialog.h"

#include <QDebug>

CddbSearchDialog::CddbSearchDialog(QWidget * parent)
: QDialog(parent), _totalTime(0) {

	_ui = new Ui::CddbSearchDialog;
	_ui->setupUi(this);

	QSettings s;

	const QString fullCddb = s.value("CDDB/URL", "http://freedb.freedb.org/~cddb/cddb.cgi").toString();
	const QSize size = s.value("CDDB/DialogSize").toSize();
	const QPoint pos = s.value("CDDB/DialogPosition").toPoint();

	if (!size.isNull())
		resize(size);

	if (!pos.isNull())
		move(pos);

	_cddbModel = new QStandardItemModel(this);
	_cddb = new Cddb(this);

	if (fullCddb.startsWith("http://", Qt::CaseInsensitive)) {
		_cddb->setHost(fullCddb.section('/', 0, 2));
		_cddb->setPath(QString("/") + fullCddb.section('/', 3));
	} else {
		_cddb->setHost(fullCddb.section('/', 0, 0));
		_cddb->setPath(QString("/") + fullCddb.section('/', 1));
	}

	_cddbModel->insertColumns(0, 5);
	_cddbModel->setHorizontalHeaderLabels(
		QStringList() << tr("Artist / Album") << tr("Category") << tr("Disc ID")
		              << tr("#") << tr("Title")
	);

	_ui->tableSearchResults->setModel(_cddbModel);
	_ui->tableSearchResults->horizontalHeader()->resizeSection(0, 250);
	_ui->tableSearchResults->horizontalHeader()->hideSection(2);
	_ui->tableSearchResults->horizontalHeader()->hideSection(3);
	_ui->tableSearchResults->horizontalHeader()->hideSection(4);

	_ui->tableTrackListing->setModel(_cddbModel);
	_ui->tableTrackListing->horizontalHeader()->resizeSection(3, 30);
	_ui->tableTrackListing->horizontalHeader()->hideSection(0);
	_ui->tableTrackListing->horizontalHeader()->hideSection(1);
	_ui->tableTrackListing->horizontalHeader()->hideSection(2);

	_ui->progressBar->setVisible(false);

	connect(_ui->buttonSearchA, SIGNAL(clicked()),
		SLOT(search()));

	connect(_ui->buttonSearchM, SIGNAL(clicked()),
		SLOT(search()));

	connect(_ui->lineSearchString, SIGNAL(returnPressed()),
		SLOT(search()));

	connect(_ui->tableSearchResults, SIGNAL(clicked(const QModelIndex&)),
		SLOT(getAlbumData(const QModelIndex&)));

	connect(_cddb, SIGNAL(albumSearchFinished(const CddbAlbumInfoList &, int, int)),
		SLOT(showMatches(const CddbAlbumInfoList &, int, int)));

	connect(_cddb, SIGNAL(trackListingFinished(const CddbTrackListing &)),
		SLOT(showTrackListing(const CddbTrackListing &)));

	connect(_ui->groupTagButtons, SIGNAL(buttonClicked(QAbstractButton*)),
		SLOT(toggleTagButtonExclusion(QAbstractButton*)));

	connect(_ui->groupSearchFieldButtons, SIGNAL(buttonClicked(QAbstractButton*)),
		SLOT(toggleSearchFieldButtonExclusion(QAbstractButton*)));

	connect(_ui->buttonsDialog->button(QDialogButtonBox::Apply), SIGNAL(clicked()),
		SLOT(accept()));

	connect(_cddb, SIGNAL(queryError(int, const QString&)),
		SLOT(showError(int, const QString&)));
}

CddbSearchDialog::~CddbSearchDialog() {
	delete _ui;
}





void CddbSearchDialog::accept() {
	if (_indexes.isEmpty() || !_dataModel)
		return;

	const QModelIndex cur  = _ui->tableSearchResults->currentIndex();
	const QString discId = cur.sibling(cur.row(), 2).data(Qt::EditRole).toString();

	bool multiArtist = _ui->checkMultiArtist->isChecked();
	bool setAll = _ui->buttonAllTags->isChecked();
	bool setArtist = _ui->buttonArtistTag->isChecked();
	bool setAlbum = _ui->buttonAlbumTag->isChecked();
	bool setGenre = _ui->buttonGenreTag->isChecked();
	bool setNumber = _ui->buttonNumberTag->isChecked();
	bool setMaxNumber = _ui->buttonNumberTag->isChecked();
	bool setTitle = _ui->buttonTitleTag->isChecked();
	bool setYear = _ui->buttonYearTag->isChecked();

	const CddbTrackListing info = _cache.value(discId);

	int maxNum = info.tracks.count();

	QString artist = info.albumInfo.title.section(" / ", 0, 0);
	QString album = info.albumInfo.title.section(" / ", 1);

	QModelIndex dataIdx = cur.sibling(cur.row(), 2).child(0, 4);

	if (!dataIdx.isValid())
		return;

	for (int i = 0; i < _indexes.count(); i++) {
		const QModelIndex idx = _indexes[i];

		if (!idx.isValid())
			continue;

		QString title = dataIdx.data().toString();

		if (multiArtist) {
			artist = title.section(" / ", 0, 0);
			title = title.section(" / ", 1);
		}

		if (setAll || setArtist)
			_dataModel->setData(idx.sibling(idx.row(), Coquillo::fieldNames.key("Artist")), artist);

		if (setAll || setAlbum)
			_dataModel->setData(idx.sibling(idx.row(), Coquillo::fieldNames.key("Album")), album);

		if (setAll || setGenre)
			_dataModel->setData(idx.sibling(idx.row(), Coquillo::fieldNames.key("Genre")), info.genre);

		if (setAll || setNumber)
			_dataModel->setData(idx.sibling(idx.row(), Coquillo::fieldNames.key("Number")), i+1);

		if (setAll || setMaxNumber)
			_dataModel->setData(idx.sibling(idx.row(), Coquillo::fieldNames.key("MaxNumber")), maxNum);

		if (setAll || setTitle)
			_dataModel->setData(idx.sibling(idx.row(), Coquillo::fieldNames.key("Title")), title);

		if (setAll || setYear)
			_dataModel->setData(idx.sibling(idx.row(), Coquillo::fieldNames.key("Year")), info.year);

		dataIdx = dataIdx.sibling(dataIdx.row()+1, dataIdx.column());
	}
}

void CddbSearchDialog::clearCddbData() {
	_cddbModel->removeRows(0, _cddbModel->rowCount());

	_ui->checkMultiArtist->setChecked(false);
	_ui->labelDiscId->setText("--");

	_indexes.clear();
	_totalTime = 0;
	_offsets.clear();
	_discId.clear();
}

void CddbSearchDialog::setIndexes(const QList<QModelIndex> & indexes) {
	_ui->labelInfo->clear();

	clearCddbData();

	if (indexes.isEmpty()) {
		_ui->buttonSearchA->setDisabled(true);
		return;
	}

	_ui->buttonSearchA->setEnabled(true);

	quint32 firstLen = 0;
	QString hex;

	for (int i = 0; i < indexes.count(); i++) {
		const QPersistentModelIndex idx = indexes[i].sibling(indexes[i].row(), Coquillo::fieldNames.key("Length"));
		quint32 length = idx.data(Qt::EditRole).toULongLong();

		if (i == 0)
			firstLen = length;

		_offsets << _totalTime * 75;
		_totalTime += length;

		_indexes << idx;
	}

	hex.setNum((_totalTime-firstLen)%255, 16);
	_discId.append( hex.rightJustified(2, '0') );

	hex.setNum(_totalTime, 16);
	_discId.append( hex.rightJustified(4, '0') );

	hex.setNum(_offsets.count(), 16);
	_discId.append( hex.rightJustified(2, '0') );

	_ui->labelDiscId->setText(_discId);
}






void CddbSearchDialog::search() {

	showBusyIndicator(true);

	switch (_ui->tabSearchMethod->currentIndex()) {
		case 0: {
			const QString id = _ui->labelDiscId->text();
			_cddb->sendAlbumSearchQuery(id, _offsets, _totalTime);

			break;
		}

		case 1: {
			const QString words = _ui->lineSearchString->text();

			CddbFields fields = Cddb::AllFields;

			if (_ui->buttonSearchArtist->isChecked())
				fields |= Cddb::ArtistField;

			if (_ui->buttonSearchAlbum->isChecked())
				fields |= Cddb::AlbumField;

			if (_ui->buttonSearchTitle->isChecked())
				fields |= Cddb::TitleField;

			_cddb->sendSearchQuery(words, fields);
			break;

		}

	}
}

void CddbSearchDialog::getAlbumData(const QModelIndex & idx) {
	if (!idx.isValid())
		return;

	const QModelIndex treeIdx = idx.sibling(idx.row(), 2);

	// Album is cached already -> skip downloading.
	if (_cddbModel->rowCount(idx.sibling(idx.row(), 2))) {
		_ui->tableTrackListing->setRootIndex(treeIdx);
		return;
	}

	QStandardItem * item = _cddbModel->itemFromIndex(treeIdx);
	item->setColumnCount(5);
	item->setRowCount(0);

	_ui->tableTrackListing->setRootIndex(treeIdx);

	const QString discId = idx.sibling(idx.row(), 2).data(Qt::EditRole).toString();
	showBusyIndicator(true);
	_cddb->sendTrackListingQuery(discId, idx.sibling(idx.row(), 1).data(Qt::EditRole).toString());
}

void CddbSearchDialog::showError(int code, const QString & message) {
	showBusyIndicator(false);

	qWarning() << "Query error:" << code << message;

	QErrorMessage * d = new QErrorMessage(this);
	d->setAttribute(Qt::WA_DeleteOnClose);
	d->showMessage(QString("Error %1: %2").arg(code).arg(message), QString("CDDB_ERROR_%1").arg(code));
}

void CddbSearchDialog::showMatches(const CddbAlbumInfoList & firstPage, int total, int pages) {
	showBusyIndicator(false);

	if (total > 0)
		_ui->labelInfo->setText(tr("%1 results in %2 page(s)").arg(total).arg(pages));
	else
		_ui->labelInfo->setText(tr("No results"));

	_cddbModel->removeRows(0, _cddbModel->rowCount());

	foreach (const CddbAlbumInfo info, firstPage) {
		QList<QStandardItem*> items;
		items << new QStandardItem(info.title);
		items << new QStandardItem(info.category);
		items << new QStandardItem(info.discId);

		items[2]->setColumnCount(5);

		_cddbModel->appendRow(items);
	}

	// Set root index to prevent the track list table from displaying albums.
	_ui->tableTrackListing->setRootIndex(_cddbModel->index(0,2));
}


void CddbSearchDialog::showTrackListing(const CddbTrackListing & list) {
	showBusyIndicator(false);

	if (!_cache.contains(list.albumInfo.discId)) {
		_cache.insert( list.albumInfo.discId, list );
	}

	QList<QStandardItem*> items = _cddbModel->findItems(list.albumInfo.discId, Qt::MatchExactly, 2);
	QStandardItem * item = items.value(0);

	if (item) {
		item->removeRows(0, item->rowCount());
		item->setRowCount(list.tracks.count());

		for (int i = 0; i < list.tracks.count(); i++) {
			item->setChild( i, 3, new QStandardItem(QString::number(i+1)) );
			item->setChild( i, 4, new QStandardItem(list.tracks[i]) );

			item->child(i, 3)->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
		}
	} else
		return;

	_ui->tableTrackListing->setRootIndex( _cddbModel->indexFromItem(items[0]) );
	_ui->checkMultiArtist->setChecked( guessMultiArtist(list) );
}

void CddbSearchDialog::toggleTagButtonExclusion(QAbstractButton * button) {
	QList<QAbstractButton*> buttons = _ui->groupTagButtons->buttons();

	if (button == _ui->buttonAllTags) {
		_ui->groupTagButtons->setExclusive( button->isChecked() );
		for (int i = 1; i < buttons.count(); i++)
			buttons[i]->setChecked(false);
	} else {
		_ui->groupTagButtons->setExclusive(false);
		_ui->buttonAllTags->setChecked(false);
	}

	if (!_ui->groupTagButtons->checkedButton())
		_ui->buttonAllTags->setChecked(true);
}

void CddbSearchDialog::toggleSearchFieldButtonExclusion(QAbstractButton * button) {
	QList<QAbstractButton*> buttons = _ui->groupSearchFieldButtons->buttons();

	if (button == _ui->buttonSearchAll) {
		_ui->groupSearchFieldButtons->setExclusive( button->isChecked() );
		for (int i = 1; i < buttons.count(); i++)
			buttons[i]->setChecked(false);
	} else {
		_ui->groupSearchFieldButtons->setExclusive(false);
		_ui->buttonSearchAll->setChecked(false);
	}

	if (!_ui->groupSearchFieldButtons->checkedButton())
		_ui->buttonSearchAll->setChecked(true);
}




void CddbSearchDialog::closeEvent(QCloseEvent * event) {
	QSettings s;
	s.setValue("CDDB/DialogSize", size());
	s.setValue("CDDB/DialogPosition", pos());

	QDialog::closeEvent(event);
}






bool CddbSearchDialog::guessMultiArtist(const CddbTrackListing & list) const {
	const QRegExp rx("( [/-] )");

	for (int i = 0; i < list.tracks.count(); i++)
		if (!list.tracks[i].contains(rx))
			return false;

	return true;
}

void CddbSearchDialog::showBusyIndicator(bool state) {
	_ui->progressBar->setRange(0, 0);
	_ui->progressBar->setVisible(state);

	_ui->buttonSearchA->setEnabled(!state);
	_ui->buttonSearchM->setEnabled(!state);
}
