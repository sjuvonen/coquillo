#include <taglib/attachedpictureframe.h>

#include <QDebug>

#include <QAbstractItemModel>
#include <QDataWidgetMapper>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QSignalMapper>
#include <QSortFilterProxyModel>

#include <core/MetaData.h>
#include <core/MetaDataImage.h>
#include <core/MetaDataModel2.h>

#include "EditorWidget.h"
#include "MetaDataChangeIndicatorDelegate.h"
#include "ui_EditorWidget.h"

typedef MetaDataModel2 MetaDataModel;

typedef TagLib::ID3v2::AttachedPictureFrame APFrame;

EditorWidget::EditorWidget(QWidget * parent)
: DataWidget(parent) {
	_ui = new Ui::EditorWidget();
	_ui->setupUi(this);

	_ui->exportImage->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));

	_ui->autoNumbers->setIcon(QIcon::fromTheme(
		"view-sort-ascending",
		style()->standardPixmap(QStyle::SP_ArrowDown)
	));

	_mapper = new QDataWidgetMapper(this);
	_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

	_mapper2 = new QDataWidgetMapper(this);
	_mapper2->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
	_mapper2->setItemDelegate(new MetaDataChangeIndicatorDelegate());

	QSignalMapper * radioMapper = new QSignalMapper(this);

	radioMapper->setMapping(_ui->applyTitle, MetaData::TitleField);
	radioMapper->setMapping(_ui->applyArtist, MetaData::ArtistField);
	radioMapper->setMapping(_ui->applyAlbum, MetaData::AlbumField);
	radioMapper->setMapping(_ui->applyGenre, MetaData::GenreField);
	radioMapper->setMapping(_ui->applyComment, MetaData::CommentField);
	radioMapper->setMapping(_ui->applyUrl, MetaData::UrlField);
	radioMapper->setMapping(_ui->applyDisc, MetaData::DiscNumberField);
	radioMapper->setMapping(_ui->applyMaxNumber, MetaData::MaxNumberField);
	radioMapper->setMapping(_ui->applyYear, MetaData::YearField);
	radioMapper->setMapping(_ui->applyImages, MetaData::PicturesField);
	radioMapper->setMapping(_ui->applyOriginalArtist, MetaData::OriginalArtistField);
	radioMapper->setMapping(_ui->applyEncoder, MetaData::EncoderField);
    radioMapper->setMapping(_ui->applyComposer, MetaData::ComposerField);
    radioMapper->setMapping(_ui->applyAlbumArtist, MetaData::AlbumArtistField);

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

	QStringList strings = _typeStrings.values();
	strings.sort();

	setDisabled(true);

	foreach (const QString s, strings)
		_ui->imageType->addItem(s, _typeStrings.key(s));

	foreach (QRadioButton * button, findChildren<QRadioButton*>())
		connect(button, SIGNAL(clicked()), radioMapper, SLOT(map()));

	foreach (QLineEdit * input, findChildren<QLineEdit*>())
		connect(input, SIGNAL(textEdited(QString)), this, SLOT(submitChanges()));

	foreach (QSpinBox * input, findChildren<QSpinBox*>())
		connect(input, SIGNAL(valueChanged(int)), _mapper, SLOT(submit()));

	connect(radioMapper, SIGNAL(mapped(int)), SLOT(copyField(int)));

	// Hide the Extra tab because needed functionality is not yet implemented.
 	_ui->tabs->removeTab(2);

}

EditorWidget::~EditorWidget() {
	delete _ui;
}

void EditorWidget::setModel(QAbstractItemModel * model) {
	DataWidget::setModel(model);

	_mapper->setModel(model);
	_mapper2->setModel(model);

	_mapper->addMapping(_ui->title, MetaData::TitleField);
	_mapper->addMapping(_ui->artist, MetaData::ArtistField);
	_mapper->addMapping(_ui->album, MetaData::AlbumField);
	_mapper->addMapping(_ui->genre, MetaData::GenreField);
	_mapper->addMapping(_ui->comment, MetaData::CommentField);
	_mapper->addMapping(_ui->url, MetaData::UrlField);
	_mapper->addMapping(_ui->disc, MetaData::DiscNumberField);
	_mapper->addMapping(_ui->number, MetaData::NumberField);
	_mapper->addMapping(_ui->maxnumber, MetaData::MaxNumberField);
	_mapper->addMapping(_ui->year, MetaData::YearField);
	_mapper->addMapping(_ui->originalArtist, MetaData::OriginalArtistField);
	_mapper->addMapping(_ui->composer, MetaData::ComposerField);
    _mapper->addMapping(_ui->encoder, MetaData::EncoderField);
    _mapper->addMapping(_ui->albumArtist, MetaData::AlbumArtistField);

	_mapper2->addMapping(_ui->labelTitle, MetaData::TitleField);
	_mapper2->addMapping(_ui->labelArtist, MetaData::ArtistField);
	_mapper2->addMapping(_ui->labelAlbum, MetaData::AlbumField);
	_mapper2->addMapping(_ui->labelGenre, MetaData::GenreField);
	_mapper2->addMapping(_ui->labelComment, MetaData::CommentField);
	_mapper2->addMapping(_ui->labelUrl, MetaData::UrlField);
	_mapper2->addMapping(_ui->labelDisc, MetaData::DiscNumberField);
	_mapper2->addMapping(_ui->labelNumber, MetaData::NumberField);
	_mapper2->addMapping(_ui->labelMaxNumber, MetaData::MaxNumberField);
	_mapper2->addMapping(_ui->labelYear, MetaData::YearField);
	_mapper2->addMapping(_ui->labelOriginalArtist, MetaData::OriginalArtistField);
	_mapper2->addMapping(_ui->labelComposer, MetaData::ComposerField);
    _mapper2->addMapping(_ui->labelEncoder, MetaData::EncoderField);
    _mapper2->addMapping(_ui->labelAlbumArtist, MetaData::AlbumArtistField);

	QItemSelectionModel * m = _ui->images->selectionModel();
	_ui->images->setModel(model);

	delete m;

	connect(_ui->images->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)),
		SLOT(displayImageProperties(QModelIndex)));

	connect(model, SIGNAL(rowsInserted(QModelIndex, int, int)),
		SLOT(updateImagesTabText()));

	connect(model, SIGNAL(rowsRemoved(QModelIndex, int, int)),
		SLOT(updateImagesTabText()));
}

void EditorWidget::setNetworkManager(QNetworkAccessManager * manager) {
	_networkManager = manager;
}





/*
 * PUBLIC SLOTS
 */

void EditorWidget::setSelection(const QItemSelection & selection) {
	DataWidget::setSelection(selection);

	const QModelIndex idx = rows().value(0);

	_mapper->setCurrentModelIndex(idx);
	_mapper2->setCurrentModelIndex(idx);
	_ui->images->setRootIndex(idx.sibling(idx.row(), MetaData::PicturesField));

	foreach (QLineEdit * line, findChildren<QLineEdit*>())
		line->setCursorPosition(0);

	_importPath.clear();
	_exportPath.clear();

	setDisabled(rows().isEmpty());

	// This cannot simply be connected to _mapper's signal currentIndexChanged(),
	// because that won't be emitted when selection is cleared.
	updateImagesTabText();
}





/**
 * PRIVATE SLOTS
 **/

void EditorWidget::automaticNumbering() {
	int total = model()->rowCount();

	for (int i = 0; i < total; i++) {
		const QModelIndex numberIdx = model()->index(i, MetaData::NumberField);
		const QModelIndex maxNumberIdx = model()->index(i, MetaData::MaxNumberField);

		model()->setData(numberIdx, i+1);
		model()->setData(maxNumberIdx, total);
	}
}

void EditorWidget::copyField(int field) {
	if (!model() || rows().isEmpty() || rows().value(0).isValid() == false)
		return;

	QList<QPersistentModelIndex> r = rows();
	const QModelIndex src = r[0].sibling(r[0].row(), field);

	if (field == MetaData::PicturesField) {
		QSortFilterProxyModel * proxy = qobject_cast<QSortFilterProxyModel*>(model());
		const QModelIndex src2 = proxy->mapToSource(src);

		if (!proxy)
			return;

		MetaDataModel * source = qobject_cast<MetaDataModel*>(proxy->sourceModel());

		if (!source)
			return;

		const QList<MetaDataImage> images = source->metaData(src2.row()).images();

		for (int i = 1; i < r.count(); i++) {
			source->setImages(proxy->mapToSource(r.at(i)), images);
		}

		return;
	}

	for (int i = 1; i < r.count(); i++) {
		const QModelIndex dst = r.at(i).sibling(r.at(i).row(), field);
		model()->setData(dst, src.data());
	}
}

void EditorWidget::displayImageProperties(const QModelIndex & idx) {
	int type = idx.data(MetaDataImage::MetaTypeRole).toInt();

	_ui->imageDescription->blockSignals(true);
	_ui->imageType->blockSignals(true);

	_ui->imageDescription->setText(idx.data(MetaDataImage::DescriptionRole).toString());
	_ui->imageType->setCurrentIndex(_ui->imageType->findText(_typeStrings.value(type)));

	_ui->imageDescription->blockSignals(false);
	_ui->imageType->blockSignals(false);

}

void EditorWidget::setImageDescription(const QString & text) {
	_ui->images->model()->setData(_ui->images->currentIndex(), text, MetaDataImage::DescriptionRole);
}

void EditorWidget::setImageType(const QString & typeString) {
	if (!_ui->images->model())
		return;

	if (_typeStrings.key(typeString, -1) == -1)
		return;

	_ui->images->model()->setData(_ui->images->currentIndex(), _typeStrings.key(typeString), MetaDataImage::MetaTypeRole);
}

void EditorWidget::addImage() {
	if (rows().count() == 0)
		return;

	if (_importPath.isEmpty() || !QFileInfo(_importPath).exists()) {
		const QModelIndex idx = rows().at(0);

		_importPath = QFileInfo(idx.sibling(idx.row(),
			MetaData::PathField).data(Qt::EditRole).toString()).path();
	}

	const QStringList files = QFileDialog::getOpenFileNames(
		this,
		tr("Import images..."),
		_importPath,
		"Pictures (*.jpg *.jpeg *.png *.bmp)"
	);

	if (files.isEmpty())
		return;

	_importPath = QFileInfo(files[0]).path();

	const QModelIndex root = _ui->images->rootIndex();

	QSortFilterProxyModel * model = qobject_cast<QSortFilterProxyModel*>(_ui->images->model());

	if (!model) {
		qWarning() << "EditorWidget: Item model not inherited from QSortFilterProxyModel!";
		return;
	}

	MetaDataModel * sourceModel = qobject_cast<MetaDataModel*>(model->sourceModel());

	if (!sourceModel) {
		qWarning() << "EditorWidget: Source model's source model not a MetaDataModel!";
		return;
	}

	foreach (const QString fileName, files) {
		const QImage image(fileName);

		if (!image.isNull()) {
			MetaDataImage mdi(image);
			mdi.setDescription(QFileInfo(fileName).baseName());
			sourceModel->addImage(model->mapToSource(root), mdi);
		}
	}

}

void EditorWidget::exportCurrentImage() {
	const QImage image = _ui->images->currentIndex().data(MetaDataImage::ImageRole).value<QImage>();

	if (_exportPath.isEmpty() || !QFileInfo(_exportPath).exists()) {
		const QModelIndex idx = rows().at(0);

		_exportPath = QFileInfo(idx.sibling(idx.row(),
		MetaData::PathField).data(Qt::EditRole).toString()).path();
	}

	const QString fileName = QFileDialog::getSaveFileName(
		this, tr("Export image..."), _exportPath);

	if (!fileName.isEmpty()) {
		bool ok = image.save(fileName);
		_exportPath = QFileInfo(fileName).path();

		if (!ok) {
			QMessageBox::critical(this, tr("Export failed"), tr("Could not save image"));
		}
	}
}

void EditorWidget::removeCurrentImage() {
	const QModelIndex idx = _ui->images->currentIndex();

	_ui->images->model()->removeRow(idx.row(), idx.parent());
}

void EditorWidget::submitChanges() {
	if (!sender()) {
		return;
	}

	if (!sender()->inherits("QLineEdit")) {
		_mapper->submit();
	}

	/*
	 * This is to fix an issue with QLineEdits and submitting the changes on
	 * every key press. The default behaviour is that the caret is moved to the end
	 * of the value every time QDataWidgetMapper::submit() is called and it re-reads
	 * the value from the model.
	 */

	QLineEdit * lineEdit = qobject_cast<QLineEdit*>(sender());
	int pos = lineEdit->cursorPosition();

	_mapper->submit();

	lineEdit->setCursorPosition(pos);
}

void EditorWidget::updateImagesTabText() {
	QString tabText = _ui->tabs->tabText(1).remove(QRegExp(" \\(\\d+\\)"));

	if (rows().isEmpty()) {
		_ui->tabs->setCurrentIndex(0);
	} else {
		const QModelIndex idx = rows()[0];
		int rowCount = model()->rowCount(idx.sibling(idx.row(), MetaData::PicturesField));

		if (rowCount) {
			tabText += QString(" (%1)").arg(rowCount);
		}
	}

	_ui->tabs->setTabText(1, tabText);
}

void EditorWidget::updateMetaDataChangeIndicators() {
	qDebug() << "update";
}
