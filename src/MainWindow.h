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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndexList>

#include "def_MetaData.h"

class QItemSelection;
class QThread;

class CddbSearchDialog;
class MediaScanner;
class MetaDataModel;
class MetaDataWriter;
class ModelDataInspector;
class ProcessorWidget;

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT

	public:
		MainWindow(QWidget * parent=0);
		~MainWindow();

		QMenu * createPopupMenu();

		void openDirectory(const QString & path);

	protected:
		void closeEvent(QCloseEvent *);
		bool eventFilter(QObject *, QEvent *);

	public slots:
		void setToolBarIconSize(int size);
		void setToolBarButtonStyle(int style);

	signals:
		void selectedRowsChanged(const QModelIndexList &);

	private slots:
		void check();

		void abortScan();
		void toggleCddbSearchDialog(bool open=false);
		void openSettingsDialog();
		void saveMetaData();
		void scanStarted();
		void scanFinished();
		void setInterfaceLocked(bool);
		void setInterfaceObjectVisible(bool);
		void setRecursiveScan(bool);
		void showAboutDialog();
		void showToolBarContextMenu(const QPoint &);
		void showHeaderContextMenu(const QPoint &);

		void slotSelectionChanged(const QItemSelection &);
		void slotSelectInverse();

	private:
		void closeCddbDialog();
		void loadSettings();
		void saveSettings();

		QThread * _scannerThread;

		CddbSearchDialog * _cddbDialog;
		MetaDataModel * _dataModel;
		ModelDataInspector * _inspector;
		MediaScanner * _scanner;
		MetaDataWriter * _writer;
		ProcessorWidget * _processor;

		Ui::MainWindow * _ui;
};

#endif
