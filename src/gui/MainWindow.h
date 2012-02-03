#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class BookmarkModel;
class CddbSearchDialog;
class ProcessorWidget;

class MetaDataModel2;
typedef MetaDataModel2 MetaDataModel;

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT

	public:
		MainWindow(MetaDataModel * model, QWidget * parent=0);
		~MainWindow();

		QMenu * createPopupMenu();

		QList<QToolBar*> toolBars() const;
		QList<QDockWidget*> dockWidgets() const;
		
		bool eventFilter(QObject * object, QEvent * event);
		
	public slots:
		void saveSettings();
		void setInterfaceLocked(bool state);
		void setVisible(bool state);

	protected:
		void showEvent(QShowEvent *);

	private slots:
		
		/**
		 * Fixes the reported selection of QItemSelectionModel's selectionChanged()
		 *
		 * By default QItemSelectionModel's signal ignores the first selected index
		 * when using Shift+Click or Ctrl+Click. Have to proxy the selection change
		 * to capture all currently selected indexes.
		 **/
		void proxyTableSelection();

		/**
		 * Invert selection in the track list.
		 **/
		void selectInverse();
		
		void selectNextIndex();
		void selectPreviousIndex();

		void setupBookmarkMenu();

		void showAboutDialog();
		void showBookmarkDialog();
		void showSettingsDialog();
		
		void showHeaderContextMenu(const QPoint & pos);

		/**
		 * Displays a progress bar for an ongoing operation in the MetaDataModel.
		 **/
		void afterModelAction();
		void beforeModelAction();

	private:
		void loadSettings();
		void setMetaDataModel(MetaDataModel * model);
		void setupToolBar();
		
		Ui::MainWindow * _ui;

		BookmarkModel * _bookmarks;
		MetaDataModel * _model;
		CddbSearchDialog * _cddbSearch;
		ProcessorWidget * _processor;

};

#endif