/***********************************************************************
* Copyright (c) 2011 Samu Juvonen <samu.juvonen@gmail.com>
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
************************************************************************/

#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H

#include <QPersistentModelIndex>
#include <QWidget>

#include "def_MetaData.h"

#include <QDataWidgetMapper>
#include "MetaDataImage.h"

class QAbstractButton;
class QAbstractItemModel;
class QModelIndex;
class QTabWidget;

namespace Ui {
	class TagEditorBasics;
}

class QTextEdit;

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
