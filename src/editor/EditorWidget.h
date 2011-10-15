#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H

#include <QPersistentModelIndex>
#include <QDataWidgetMapper>
#include <QWidget>

#include "MetaDataImage.h"

class QAbstractButton;
class QAbstractItemModel;
class QModelIndex;
class QTabWidget;

namespace Ui {
	class TagEditorBasics;
}

class PicturesTab;

class EditorWidget : public QWidget {
	Q_OBJECT

	public:
		EditorWidget(QWidget * parent=0);
		~EditorWidget();

		void setModel(QAbstractItemModel * source);

		QDataWidgetMapper * dataMapper() const { return _mapper; }

	public slots:
		void setDataIndexes(const QModelIndexList &);

	private slots:
		void cloneButtonPressed(QAbstractButton *);
		void setImages(const QList<MetaDataImage> &);
		void updatePictures();

		void slotApplyPicturesToAll();
		void cloneValue(int section, const QVariant & value);

		void scrollTextFields();

	private:
		static QList<QVariant> picturesToVariants(const QList<MetaDataImage> & pics);
		static QList<MetaDataImage> variantsToPictures(const QVariantList & pics);

		void updatePicturesTabText(int pictureCount);

		QAbstractItemModel * _model;
		QList<QPersistentModelIndex> _indexes;

		QTabWidget * _tabs;
		PicturesTab * _picturesTab;

		QDataWidgetMapper * _mapper;

		Ui::TagEditorBasics * _basicsTabUi;
};

#endif
