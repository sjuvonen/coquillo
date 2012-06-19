#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H

#include <QPersistentModelIndex>

#include "DataWidget.h"

class QAbstractItemModel;
class QDataWidgetMapper;
class QNetworkAccessManager;
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
		void setNetworkManager(QNetworkAccessManager * manager);

	public slots:
		void setSelection(const QItemSelection & selection);

	private slots:
		void automaticNumbering();
		void copyField(int field);
		void displayImageProperties(const QModelIndex & idx);
		void setImageDescription(const QString & text);
		void setImageType(const QString & typeString);

		void addImage();
		void exportCurrentImage();
		void removeCurrentImage();

		void submitChanges();

		void updateImagesTabText();
		void updateMetaDataChangeIndicators();

	private:
		Ui::EditorWidget * _ui;
		QDataWidgetMapper * _mapper;
		QDataWidgetMapper * _mapper2;
		QNetworkAccessManager * _networkManager;
		
		QHash<int, QString> _typeStrings;
		QString _importPath;
		QString _exportPath;
};

#endif
