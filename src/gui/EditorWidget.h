#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H

#include <QPersistentModelIndex>

#include "DataWidget.h"

class QAbstractItemModel;
class QDataWidgetMapper;
class QItemSelection;

namespace Ui {
	class EditorWidget;
}

class EditorWidget : public DataWidget {
	Q_OBJECT

	public:
		EditorWidget(QWidget * parent=0);
		~EditorWidget();

		void setModel(QAbstractItemModel * model);

	public slots:
		void setSelection(const QItemSelection & selection);

	private slots:
		void copyField(int field);
		void displayImageProperties(const QModelIndex & idx);
		void setImageDescription(const QString & text);
		void setImageType(const QString & typeString);

		void addImage();
		void exportCurrentImage();
		void removeCurrentImage();

		void updateImagesTabText();

	private:
		Ui::EditorWidget * _ui;
		QDataWidgetMapper * _mapper;
		
		QHash<int, QString> _typeStrings;
		QString _importPath;
		QString _exportPath;
};

#endif