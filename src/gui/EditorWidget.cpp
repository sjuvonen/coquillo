#include <taglib/attachedpictureframe.h>

#include <QDebug>

#include <QAbstractItemModel>
#include <QDataWidgetMapper>
#include <QFileDialog>
#include <QFileInfo>
#include <QSignalMapper>
#include <QSortFilterProxyModel>

#include <core/MetaData.h>
#include <core/MetaDataImage.h>
#include <core/MetaDataModel2.h>

#include "EditorWidget.h"
#include "ui_EditorWidget.h"

typedef MetaDataModel2 MetaDataModel;

typedef TagLib::ID3v2::AttachedPictureFrame APFrame;

EditorWidget::EditorWidget(QWidget * parent)
: DataWidget(parent) {
	_ui = new Ui::EditorWidget();
	_ui->setupUi(this);

	_mapper = new QDataWidgetMapper(this);
	_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

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
	
	foreach (QRadioButton * button, findChildren<QRadioButton*>())
		connect(button, SIGNAL(clicked()), radioMapper, SLOT(map()));

	foreach (QLineEdit * input, findChildren<QLineEdit*>())
		connect(input, SIGNAL(textEdited(QString)), _mapper, SLOT(submit()));

	foreach (QSpinBox * input, findChildren<QSpinBox*>())
		connect(input, SIGNAL(valueChanged(int)), _mapper, SLOT(submit()));

	connect(radioMapper, SIGNAL(mapped(int)), SLOT(copyField(int)));

	setDisabled(true);

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

	foreach (const QString s, strings)
		_ui->imageType->addItem(s, _typeStrings.key(s));

	_ui->exportImage->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
	
}

EditorWidget::~EditorWidget() {
	delete _ui;
}

void EditorWidget::setModel(QAbstractItemModel * model) {
	DataWidget::setModel(model);
	
	_mapper->setModel(model);

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

	QItemSelectionModel * m = _ui->images->selectionModel();
	_ui->images->setModel(model);

	connect(_ui->images->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)),
		SLOT(displayImageProperties(QModelIndex)));

	delete m;
}





/**
 * PUBLIC SLOTS
 **/

void EditorWidget::setSelection(const QItemSelection & selection) {
	DataWidget::setSelection(selection);

	const QModelIndex idx = rows().value(0);

	_mapper->setCurrentModelIndex(idx);

	_ui->images->setRootIndex(idx.sibling(idx.row(), MetaData::PicturesField));
	
	setDisabled(rows().isEmpty());

	foreach (QLineEdit * line, findChildren<QLineEdit*>())
		line->setCursorPosition(0);
}





/**
 * PRIVATE SLOTS
 **/

void EditorWidget::copyField(int field) {
	if (!model() || rows().isEmpty() || rows().value(0).isValid() == false)
		return;

	QList<QPersistentModelIndex> r = rows();
	const QModelIndex src = r[0].sibling(r[0].row(), field);

	for (int i = 1; i < r.count(); i++) {
		const QModelIndex dst = r[i].sibling(r[i].row(), field);

		if (field == MetaData::PicturesField) {
			int row = model()->rowCount(dst);
			
			for (int j = 0; j < model()->rowCount(src); j++) {
				model()->insertRow(row+j, dst);
				model()->setData(dst.child(row+j, 0), src.child(row+j, 0).data());
				model()->setData(dst.child(row+j, 1), src.child(row+j, 1).data());

				// Have to set data twice so that the model would mark the row as changed...
				model()->setData(dst.child(row+j, 2), src.child(row+j, 2).data());
				model()->setData(dst.child(row+j, 2), src.child(row+j, 2).data());
			}
			
		} else {
			model()->setData(dst, src.data());
		}
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
	const QStringList files = QFileDialog::getOpenFileNames(
		this,
		QString(),
		QString(),
		"Pictures (*.jpg *.jpeg *.png *.bmp)"
	);

	if (files.isEmpty())
		return;

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

		qDebug() << QFileInfo(fileName).baseName();
		
		if (!image.isNull()) {
			MetaDataImage mdi(image);
			mdi.setDescription(QFileInfo(fileName).baseName());
			sourceModel->addImage(model->mapToSource(root), mdi);
		}
	}

}

void EditorWidget::exportCurrentImage() {
	const QImage image = _ui->images->currentIndex().data(MetaDataImage::ImageRole).value<QImage>();

	const QString fileName = QFileDialog::getSaveFileName(
		this, QString(), QString(), tr("Picture(*.jpg *.png *.bmp)")
	);

	if (!fileName.isEmpty())
		image.save(fileName);
}

void EditorWidget::removeCurrentImage() {
	const QModelIndex idx = _ui->images->currentIndex();

	_ui->images->model()->removeRow(idx.row(), idx.parent());
}
