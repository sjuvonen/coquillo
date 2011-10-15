#include <taglib/attachedpictureframe.h>

#include <QFileDialog>
#include <QModelIndex>

#include <metadata/ImageCache.h>
#include <metadata/MetaDataImage.h>
#include <metadata/MetaDataModel.h>
#include <metadata/MetaDataReader.h>

#include "PictureListDelegate.h"
#include "PicturesTab.h"

#include "ui_TagEditorPictures.h"

#include <QDebug>

typedef TagLib::ID3v2::AttachedPictureFrame APFrame;

PicturesTab::PicturesTab(QWidget * parent) : QWidget(parent) {
	_ui = new Ui::TagEditorPictures;

	_ui->setupUi(this);

	_pictureModel = new QStandardItemModel(this);

	_ui->ImagesList->setItemDelegate( new PictureListDelegate(this) );
	_ui->ImagesList->setModel(_pictureModel);

	_typeStrings.insert(0, tr("Other"));
	_typeStrings.insert(APFrame::FrontCover, tr("Front cover"));
	_typeStrings.insert(APFrame::BackCover, tr("Back cover"));
	_typeStrings.insert(APFrame::LeafletPage, tr("Leaflet page"));
	_typeStrings.insert(APFrame::Media, tr("Media"));
	_typeStrings.insert(APFrame::LeadArtist, tr("Lead artist"));
	_typeStrings.insert(APFrame::Artist, tr("Artist"));
	_typeStrings.insert(APFrame::Conductor, tr("Conductor"));
	_typeStrings.insert(APFrame::Band, tr("Band"));
	_typeStrings.insert(APFrame::Lyricist, tr("Lyricist"));
	_typeStrings.insert(APFrame::RecordingLocation, tr("Recording location"));
	_typeStrings.insert(APFrame::DuringRecording, tr("During recording"));
	_typeStrings.insert(APFrame::DuringPerformance, tr("During performance"));
	_typeStrings.insert(APFrame::MovieScreenCapture, tr("Movie screen capture"));
	_typeStrings.insert(APFrame::Illustration, tr("Illustration"));
	_typeStrings.insert(APFrame::BandLogo, tr("Band logo"));
	_typeStrings.insert(APFrame::PublisherLogo, tr("Publisher logo"));

	QStringList types = _typeStrings.values();
	types.sort();

	foreach (QString t, types)
		_ui->PictureTypeSelector->addItem(t, _typeStrings.key(t));

	connect(_ui->DeletePictureButton, SIGNAL(clicked()), this, SLOT(deleteCurrentPicture()));
	connect(_ui->AddPictureButton, SIGNAL(clicked()), this, SLOT(addPictures()));
	connect(_ui->SavePictureButton, SIGNAL(clicked()), this, SLOT(savePicture()));
	connect(_ui->ApplyPicturesButton, SIGNAL(clicked()), this, SIGNAL(applyPicturesToAll()));

	connect(_ui->ImagesList, SIGNAL(clicked(const QModelIndex &)),
		this, SLOT(selectedPictureChanged(const QModelIndex &)));

	connect(_ui->PictureTypeSelector, SIGNAL(activated(int)),
		this, SLOT(changePictureType(int)));

	connect(_ui->PictureTypeSelector, SIGNAL(editTextChanged(const QString &)),
		this, SLOT(changePictureType(const QString &)));

	connect(_ui->PictureDescriptionLine, SIGNAL(textEdited(const QString &)),
		this, SLOT(changePictureDescription(const QString &)));
}

void PicturesTab::changePictureDescription(const QString & descr) {
	const QModelIndex idx = _ui->ImagesList->currentIndex();
	int id = idx.data(PictureIdRole).toInt();
	int type = idx.data(PictureTypeRole).toInt();

	_pictureModel->setData(idx, descr);

	const MetaDataImage pic(id, type, descr);

	emit pictureChanged(pic);
}

void PicturesTab::changePictureType(int index) {
	const QModelIndex idx = _ui->ImagesList->currentIndex();
	const QString descr = idx.data().toString();
	int id = idx.data(PictureIdRole).toInt();

	int type = _ui->PictureTypeSelector->itemData(index).toInt();

	_pictureModel->setData(idx, type, PictureTypeRole);

	if (type == -1) {
		int row = _ui->PictureTypeSelector->findData(-1);
		const QString typeString = _ui->PictureTypeSelector->itemText(row);
		_pictureModel->setData(idx, typeString, PictureTypeStringRole);
	} else
		_pictureModel->setData(idx, _typeStrings.value(type), PictureTypeStringRole);

	emit pictureChanged(MetaDataImage(id, type, descr));
}

void PicturesTab::changePictureType(const QString & customText) {
	const int customType = -1;
	int row =_ui->PictureTypeSelector->findData(customType);
	_ui->PictureTypeSelector->setItemText(row, customText);

	changePictureType(row);
}

QList<MetaDataImage> PicturesTab::pictures() const {
	QList<MetaDataImage> imgs;

	for (int i = 0; i < _pictureModel->rowCount(); i++) {
		const QModelIndex idx = _pictureModel->index(i, 0);
		const QString description = idx.data().toString();
		int type = idx.data(PictureTypeRole).toInt();
		QString typeString = idx.data(PictureTypeStringRole).toString();
		int id = idx.data(PictureIdRole).toInt();

		if (type != -1)
			imgs << MetaDataImage(id, type, description);
		else
			imgs << MetaDataImage(id, typeString, description);
	}

	return imgs;
}



void PicturesTab::clearPictures() {
	_pictureModel->clear();
}

void PicturesTab::setPictures(const QList<MetaDataImage> & pics) {
	_pictureModel->clear();

	foreach(const MetaDataImage pic, pics)
		addPictureToList(pic);

	_ui->ImagesList->setCurrentIndex(_pictureModel->index(0, 0));
	selectedPictureChanged(_pictureModel->index(0, 0));

	_ui->SavePictureButton->setEnabled( !pics.isEmpty() );

}

void PicturesTab::addPictures() {
	const QStringList files = QFileDialog::getOpenFileNames(
		this,
		QString(),
		QString(),
		"Pictures (*.jpg *.jpeg *.png *.bmp)"
	);

	if (files.isEmpty())
		return;

	foreach (const QString file, files) {
		const QImage img(file);

		if (!img.isNull()) {
			_ui->SavePictureButton->setEnabled(true);
			MetaDataImage pic(img);

			addPictureToList(pic);

			emit pictureAdded(pic);
		}
	}

	if (!_ui->ImagesList->currentIndex().isValid()) {
		_ui->ImagesList->setCurrentIndex(_pictureModel->index(0,0));
		selectedPictureChanged(_pictureModel->index(0, 0));
	}
}

void PicturesTab::savePicture() {
	const QModelIndex idx = _ui->ImagesList->currentIndex();

	if (!idx.isValid())
		return;

	int row = idx.row();
	const QImage picture = pictures()[row].image();
	const QString fileName = QFileDialog::getSaveFileName(this, QString(), QString(), tr("Picture(*.jpg *.png *.gif)"));

	if (!fileName.isEmpty()) {
		QString format = fileName.section('.', -1).toUpper();

		if (!format.contains(QRegExp("^(JPEG|PNG|BMP)$")))
			format = "JPEG";

		picture.save(fileName, format.toUtf8().constData());
	}
}

void PicturesTab::addPictureToList(const MetaDataImage & pic) {
	const QIcon icon(QPixmap::fromImage(pic.image()));

	QStandardItem * item = new QStandardItem(icon, pic.description());
	item->setData(pic.type(), PictureTypeRole);
	item->setData( pic.size(), PictureDimensionsRole );
	item->setData(QSize(100, 100), Qt::SizeHintRole);
	item->setData(pic.id(), PictureIdRole);
	item->setFlags( item->flags() ^ Qt::ItemIsEditable );

	if (pic.type() != -1)
		item->setData(_typeStrings.value(pic.type()), PictureTypeStringRole);
	else
		item->setData(pic.typeString(), PictureTypeStringRole);

	_pictureModel->appendRow( item );
}

void PicturesTab::deleteCurrentPicture() {
	const QModelIndex idx = _ui->ImagesList->currentIndex();

	if (idx.isValid()) {
		qint16 id = idx.data(PictureIdRole).toInt();

		QList<QStandardItem*> items = _pictureModel->takeRow( idx.row() );

		foreach (QStandardItem * item, items)
			if (item) delete item;

		emit pictureRemoved(id);
	}

	if (!_pictureModel->rowCount())
		_ui->SavePictureButton->setEnabled(false);

}

void PicturesTab::selectedPictureChanged(const QModelIndex & idx) {
	int type = idx.data(PictureTypeRole).toInt();
	int row = _ui->PictureTypeSelector->findData(type);

	_ui->PictureTypeSelector->blockSignals(true);

	_ui->PictureDescriptionLine->setText(idx.data().toString());
	_ui->PictureTypeSelector->setCurrentIndex(row);

	_ui->PictureTypeSelector->blockSignals(false);
}