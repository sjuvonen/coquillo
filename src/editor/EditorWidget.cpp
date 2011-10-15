
#include <QDataWidgetMapper>
#include <QSignalMapper>
#include <QSortFilterProxyModel>
#include <QTabWidget>

#include <metadata/MetaDataModel.h>
#include <metadata/MetaDataReader.h>

#include "EditorWidget.h"
#include "PicturesTab.h"

#include "ui_TagEditorBasics.h"

#include <QDebug>

// Should get rid of this somehow.
#include "globals.h"

EditorWidget::EditorWidget(QWidget * parent)
: QWidget(parent), _model(0) {

	_mapper = new QDataWidgetMapper;

	QWidget * tagsTab = new QWidget(this);
	_picturesTab = new PicturesTab(this);

	_basicsTabUi = new Ui::TagEditorBasics;
	_basicsTabUi->setupUi(tagsTab);

	_tabs = new QTabWidget(this);
	_tabs->addTab(tagsTab, tr("Tags"));
	_tabs->addTab(_picturesTab, tr("Pictures") );

	QVBoxLayout * layout = new QVBoxLayout;
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(_tabs);

	setLayout(layout);

	connect(_basicsTabUi->cloneValueButtons, SIGNAL(buttonClicked(QAbstractButton*)),
		SLOT(cloneButtonPressed(QAbstractButton*)));

	connect(_picturesTab, SIGNAL(pictureAdded(const MetaDataImage&)), SLOT(updatePictures()));
	connect(_picturesTab, SIGNAL(pictureChanged(const MetaDataImage&)), SLOT(updatePictures()));
	connect(_picturesTab, SIGNAL(pictureRemoved(qint16)), SLOT(updatePictures()));
	connect(_picturesTab, SIGNAL(applyPicturesToAll()), SLOT(slotApplyPicturesToAll()));

	// This connection doesn't work because we have to block signals for the mapper
	// to prevent another bug.
// 	connect(_mapper, SIGNAL(currentIndexChanged(int)),
// 		SLOT(scrollTextFields()));

}

EditorWidget::~EditorWidget() {
	delete _basicsTabUi;
}

void EditorWidget::slotApplyPicturesToAll() {
	if (_indexes.count() < 2)
		return;

	int col = modelColumn("Pictures");
	const QVariant pics = _indexes[0].sibling(_indexes[0].row(), col).data(Qt::EditRole);

	for (int i = 1; i < _indexes.count(); i++) {
		// Call setdata(Qt::EditRole) twice to ensure that the rows are marked as changed

		const QModelIndex idx = _indexes[i].sibling(_indexes[i].row(), col);
		_model->setData(idx, QVariant(0), Qt::EditRole);
		_model->setData(idx, pics, Qt::EditRole);
	}
}

void EditorWidget::setModel(QAbstractItemModel * model) {

	_model = model;
	_mapper->setModel(model);

	_mapper->addMapping(_basicsTabUi->fieldTitle, 0);
	_mapper->addMapping(_basicsTabUi->fieldArtist, 1);
	_mapper->addMapping(_basicsTabUi->fieldAlbum, 2);
 	_mapper->addMapping(_basicsTabUi->fieldYear, 3);
	_mapper->addMapping(_basicsTabUi->fieldDisc, 4);
	_mapper->addMapping(_basicsTabUi->fieldNumber, 5);
	_mapper->addMapping(_basicsTabUi->fieldMaxNumber, 6);
	_mapper->addMapping(_basicsTabUi->fieldGenre, 7);
	_mapper->addMapping(_basicsTabUi->fieldComment, 8);
	_mapper->addMapping(_basicsTabUi->fieldOriginalArtist, 9);
	_mapper->addMapping(_basicsTabUi->fieldComposer, 10);
	_mapper->addMapping(_basicsTabUi->fieldUrl, 11);
	_mapper->addMapping(_basicsTabUi->fieldEncoder, 12);
	_mapper->addMapping(_basicsTabUi->fieldPath, 13);
}





void EditorWidget::setDataIndexes(const QModelIndexList & indexes) {
	_indexes.clear();

	foreach (const QModelIndex idx, indexes)
		_indexes << QPersistentModelIndex(idx);

	const QModelIndex idx = indexes.value(0);

	_basicsTabUi->fieldDisc->setValue(0);
	_basicsTabUi->fieldNumber->setValue(0);
	_basicsTabUi->fieldMaxNumber->setValue(0);
	_basicsTabUi->fieldYear->setValue(0);

	_mapper->blockSignals(true);
	_mapper->setCurrentIndex(idx.row());
	scrollTextFields();
	_mapper->blockSignals(false);

	int col = modelColumn("Pictures");

	QList<MetaDataImage> pics;

	if (idx.sibling(idx.row(), col).data(Qt::EditRole).isNull()) {
		const QModelIndex pathIdx = idx.sibling(idx.row(), modelColumn("Path"));
		QString path = pathIdx.data(Coquillo::OriginalDataRole).toString();

		if (path.isEmpty())
			path = pathIdx.data(Qt::EditRole).toString();

		pics = MetaDataReader::getImages(path);
	} else
		pics = variantsToPictures( idx.sibling(idx.row(), col).data(Qt::EditRole).toList() );

	setImages( pics );

	// The data model considers every setdata(Qt::EditRole) call after the first one as modifying original data,
	// so we don't want to call this multiple times.
	if (!pics.isEmpty() && idx.sibling(idx.row(), modelColumn("Pictures")).data(Qt::EditRole).isNull())
		_model->setData(_model->index(idx.row(), modelColumn("Pictures")), picturesToVariants(pics), Qt::EditRole);

}

void EditorWidget::setImages(const QList<MetaDataImage> & images) {
	_picturesTab->setPictures(images);
	updatePicturesTabText(images.count());

}

void EditorWidget::updatePictures() {
	if (_indexes.isEmpty())
		return;

	const QModelIndex idx = _indexes[0].sibling(_indexes[0].row(), modelColumn("Pictures"));
	const QList<MetaDataImage> pics = _picturesTab->pictures();

	if (idx.isValid())
		_model->setData( idx, picturesToVariants(pics), Qt::EditRole );

	updatePicturesTabText(pics.count());
}









void EditorWidget::cloneButtonPressed(QAbstractButton * button) {
	const QString field = button->objectName().remove(QRegExp("^apply"));
	const QWidget * widget = findChild<QWidget*>(QString("field%1").arg(field));

	QString value;

	if (!widget)
		return;
	else if (widget->inherits("QLineEdit"))
		value = qobject_cast<const QLineEdit*>(widget)->text();
	else if (widget->inherits("QSpinBox"))
		value = QString::number( qobject_cast<const QSpinBox*>(widget)->value() );
	else {
		qWarning("Unknown widget type in tags page!");
		return;
	}

	for (int i = 0; i < _model->columnCount(); i++) {
		if (_model->headerData(i, Qt::Horizontal, Qt::DisplayRole) == field) {
			cloneValue(i, value);
			return;
		}
	}
}


void EditorWidget::cloneValue(int section, const QVariant & value) {
	foreach (const QModelIndex idx, _indexes)
		_model->setData(idx.sibling(idx.row(), section), value, Qt::EditRole);
}


QList<QVariant> EditorWidget::picturesToVariants(const QList<MetaDataImage> & pics) {
	QVariantList list;

	foreach (const MetaDataImage p, pics)
		list << QVariant::fromValue<MetaDataImage>(p);

	return list;
}

QList<MetaDataImage> EditorWidget::variantsToPictures(const QVariantList & pics) {
	QList<MetaDataImage> list;

	foreach (const QVariant p, pics)
		list << p.value<MetaDataImage>();

	return list;
}

void EditorWidget::updatePicturesTabText(int pictureCount) {
	int i = _tabs->indexOf(_picturesTab);

	if (pictureCount)
		_tabs->setTabText(i, tr("Pictures") + QString(" (%1)").arg(pictureCount));
	else
		_tabs->setTabText(i, tr("Pictures"));
}

void EditorWidget::scrollTextFields() {
	QList<QLineEdit*> fields = findChildren<QLineEdit*>();

	foreach (QLineEdit * field, fields)
		field->setCursorPosition(0);
}