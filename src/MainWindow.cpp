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

#include <QAction>
#include <QActionGroup>
#include <QDesktopServices>
#include <QLabel>
#include <QMenuBar>
#include <QProgressBar>
#include <QSettings>
#include <QSignalMapper>
#include <QSortFilterProxyModel>
#include <QStatusBar>
#include <QThread>
#include <QToolBar>

#include "AboutDialog.h"
#include "CddbSearchDialog.h"
#include "DirectorySelectorWidget.h"
#include "EditorWidget.h"
#include "ImageCache.h"
#include "MainWindow.h"
#include "MetaDataModel.h"
#include "MetaDataWriter.h"
#include "ModelDataInspector.h"
#include "MediaScanner.h"
#include "ProcessorWidget.h"
#include "SettingsDialog.h"
#include "SettingsDialogPage.h"
#include "TableViewMod.h"

#include "SortFilterProxyModelMod.h"

#include "globals.h"

#include "uih/ui_MainWindow.h"

#include <QDebug>

volatile bool forceAbortScan = false;

MainWindow::MainWindow(QWidget * parent)
: QMainWindow(parent), _cddbDialog(0) {

	qRegisterMetaType<MetaData>("MetaData");

	_ui = new Ui::MainWindow;
	_ui->setupUi(this);

	_dataModel = new MetaDataModel(this);

	_processor = new ProcessorWidget(this);
	_processor->setWindowFlags( Qt::Window | Qt::Dialog | Qt::Popup );

	_inspector = new ModelDataInspector(this);
	_inspector->setModel(_dataModel);

	_scannerThread = new QThread(this);

	_scanner = new MediaScanner;
	_scanner->moveToThread(_scannerThread);

	_writer = new MetaDataWriter;
	_writer->moveToThread(_scannerThread);

	_scannerThread->start();

	QSortFilterProxyModel * m = new SortFilterProxyModelMod;
	m->setDynamicSortFilter(true);
	m->setSourceModel(_dataModel);

	_ui->widgetEditor->setModel(m);
	_ui->tableItems->setModel(m);
	_processor->setModel(m);

	QProgressBar * scanProgress = new QProgressBar(this);
	scanProgress->setObjectName("ScanProgressBar");
	scanProgress->setFixedSize(QSize(140, 20));
	scanProgress->setVisible(false);

	QLabel * statusMessage = new QLabel(this);
	statusMessage->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
	statusMessage->setObjectName("StatusMessage");

	statusBar()->insertWidget(0, scanProgress);
	statusBar()->addPermanentWidget(statusMessage, 1);

	// Handles toggling the Processor button in toolbar
	_processor->installEventFilter(this);

	// Add this action to MainWindow so that the shortcut works even when menubar is hidden.
	addAction( _ui->actionMenubar );
	addAction( _ui->actionQuit );
	addAction( _ui->actionConfigure );

	_ui->tableItems->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);

	connect(_ui->tableItems->horizontalHeader(), SIGNAL(customContextMenuRequested(const QPoint &)),
		SLOT(showHeaderContextMenu(const QPoint &)));

	connect(_scanner, SIGNAL(started()), statusMessage, SLOT(clear()));
	connect(_scanner, SIGNAL(started()), _dataModel, SLOT(clearContents()));
	connect(_scanner, SIGNAL(started()), SLOT(scanStarted()));
	connect(_scanner, SIGNAL(finished()), SLOT(scanFinished()));

	connect(_scanner, SIGNAL(itemFound(const MetaData &)), _dataModel, SLOT(addMetaData(const MetaData &)));
	connect(_scanner, SIGNAL(itemsDone(int)), scanProgress, SLOT(setValue(int)));
	connect(_scanner, SIGNAL(maxItemsChanged(int)), scanProgress, SLOT(setMaximum(int)));

	connect(_ui->actionReload, SIGNAL(triggered()), _scanner, SLOT(scan()));

	connect(_ui->actionAbout, SIGNAL(triggered()), SLOT(showAboutDialog()));
	connect(_ui->actionAbort, SIGNAL(triggered()), SLOT(abortScan()));
	connect(_ui->actionConfigure, SIGNAL(triggered()), SLOT(openSettingsDialog()));
	connect(_ui->actionRecursive_scan, SIGNAL(toggled(bool)), SLOT(setRecursiveScan(bool)));

	connect(
		_ui->tableItems->selectionModel(),
		SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
		SLOT(slotSelectionChanged(const QItemSelection &))
	);

	connect(this, SIGNAL(selectedRowsChanged(const QModelIndexList &)),
		_ui->widgetEditor, SLOT(setDataIndexes(const QModelIndexList &)));

	connect(this, SIGNAL(selectedRowsChanged(const QModelIndexList &)),
		_processor, SLOT(setDataIndexes(const QModelIndexList &)));

	connect(_processor, SIGNAL(closeButtonPressed()), _processor, SLOT(close()));

	connect(_ui->actionProcessor, SIGNAL(toggled(bool)), _processor, SLOT(setVisible(bool)));

	connect(_ui->widgetLocation, SIGNAL(pathSelected(const QString &)), _scanner, SLOT(scanPath(const QString &)));
	connect(_ui->widgetLocation, SIGNAL(pathSelected(const QString &)), _dataModel, SLOT(setRootDirectory(const QString &)));

	connect(_ui->mainToolBar, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(showToolBarContextMenu(const QPoint &)));
	connect(_ui->actionSave, SIGNAL(triggered()), SLOT(saveMetaData()));
	connect(_ui->actionReset, SIGNAL(triggered()), _dataModel, SLOT(undoChanges()));

	connect(_ui->actionCDDB, SIGNAL(toggled(bool)), SLOT(toggleCddbSearchDialog(bool)));

	connect(_dataModel, SIGNAL(metaDataStateChanged(bool)), _ui->actionSave, SLOT(setEnabled(bool)));
	connect(_dataModel, SIGNAL(metaDataStateChanged(bool)), _ui->actionReset, SLOT(setEnabled(bool)));

	connect(
		_ui->actionInverse, SIGNAL(triggered()),
		SLOT(slotSelectInverse())
	);

	connect(
		_ui->actionNext, SIGNAL(triggered()),
		_ui->widgetEditor->dataMapper(), SLOT(toNext())
	);

	connect(
		_ui->actionPrevious, SIGNAL(triggered()),
		_ui->widgetEditor->dataMapper(), SLOT(toPrevious())
	);

	connect(
		_ui->widgetEditor->dataMapper(), SIGNAL(currentIndexChanged(int)),
		_ui->tableItems, SLOT(selectRow(int))
	);

	for (int i = 0; i < g_fieldNames.count(); i++)
		if (i != modelColumn("Title") && i != modelColumn("Path") && i != modelColumn("Number"))
		_ui->tableItems->horizontalHeader()->hideSection(i);

	_ui->tableItems->horizontalHeader()->moveSection(modelColumn("Number"), 0);
	_ui->tableItems->horizontalHeader()->resizeSection(modelColumn("Number"), 35);
	_ui->tableItems->horizontalHeader()->resizeSection(0, 300);

	loadSettings();

	_ui->actionPrevious->setIcon(style()->standardIcon(QStyle::SP_ArrowLeft));
	_ui->actionNext->setIcon(style()->standardIcon(QStyle::SP_ArrowRight));
	_ui->actionReset->setIcon(style()->standardIcon(QStyle::SP_DialogResetButton));
	_ui->actionSave->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
	_ui->actionAbort->setIcon(style()->standardIcon(QStyle::SP_DialogCancelButton));
	_ui->actionReload->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));

	_ui->actionCDDB->setIcon(style()->standardIcon(QStyle::SP_DriveCDIcon));
	_ui->actionProcessor->setIcon(style()->standardIcon(QStyle::SP_ComputerIcon));

	_ui->tableItems->setSelectionBehavior(QAbstractItemView::SelectRows);
}

MainWindow::~MainWindow() {
	closeCddbDialog();
	delete _ui;
}

void MainWindow::slotSelectionChanged(const QItemSelection &) {
	emit selectedRowsChanged( _ui->tableItems->selectionModel()->selectedRows() );

	/*
	 *
	QList<QModelIndex> idxs = _ui->tableItems->selectionModel()->selectedRows();
	QSortFilterProxyModel * m = qobject_cast<QSortFilterProxyModel*>(_ui->tableItems->model());

	for (int i = 0; i < idxs.count(); i++) {
		idxs[i] = m->mapToSource(idxs[i]);

		qDebug() << idxs[i].model();
	}

	emit selectedRowsChanged( idxs );
	*/
}

void MainWindow::slotSelectInverse() {
	QAbstractItemModel * m = _ui->tableItems->model();
	const QModelIndex tl = m->index(0, 0);
	const QModelIndex br = m->index(m->rowCount()-1, m->columnCount()-1);
	const QItemSelection s(tl, br);

	_ui->tableItems->selectionModel()->select(s, QItemSelectionModel::Toggle);
}



QMenu * MainWindow::createPopupMenu() {
	QMenu * m = new QMenu;
	m->setAttribute(Qt::WA_DeleteOnClose, true);

	const QList<QToolBar *> bars = findChildren<QToolBar*>();

	QAction * lock = m->addAction(tr("Lock interface"));
	lock->setCheckable(true);
	lock->setChecked(!bars.isEmpty() ? !bars.at(0)->isMovable() : false);

	m->addSeparator();
	m->addAction(_ui->actionMenubar);

	foreach (QToolBar * b, bars) {
		QAction * a = m->addAction(b->windowTitle());
		a->setCheckable(true);
		a->setChecked(b->isVisible());
		a->setData(b->objectName());

		connect(a, SIGNAL(triggered(bool)), this, SLOT(setInterfaceObjectVisible(bool)));
	}

	m->addSeparator();

	QMenu * dockMenu = m->addMenu(tr("Docks"));

	foreach (QDockWidget * d, findChildren<QDockWidget*>()) {
		QAction * a = dockMenu->addAction(d->windowTitle());
		a->setCheckable(true);
		a->setChecked(d->isVisible());
		a->setData(d->objectName());

		connect(a, SIGNAL(triggered(bool)), this, SLOT(setInterfaceObjectVisible(bool)));
	}

	connect(lock, SIGNAL(triggered(bool)), this, SLOT(setInterfaceLocked(bool)));
	//connect(m, SIGNAL(aboutToHide()), m, SLOT(deleteLater()));

	connect(m, SIGNAL(aboutToHide()), m, SLOT(deleteLater()));
	//connect(m, SIGNAL(destroyed()), this, SLOT(check()));

	return m;
}

#include <QTimer>

void MainWindow::openDirectory(const QString & path) {
	_ui->widgetLocation->setPath(path);
	_scanner->setPath(path);
	_dataModel->setRootDirectory(path);

	QTimer::singleShot(500, _scanner, SLOT(scan()));
}




void MainWindow::closeEvent(QCloseEvent *) {
	abortScan();
	saveSettings();
	_scannerThread->quit();

	closeCddbDialog();
}

bool MainWindow::eventFilter(QObject * object, QEvent * event) {

	if (object == _processor) {
		QSettings s;

		if (event->type() == QEvent::Close || event->type() == QEvent::Hide) {
			s.setValue("Processor/DialogPosition", _processor->pos());
			s.setValue("Processor/DialogSize", _processor->size());

			_ui->actionProcessor->setChecked(false);

		}
		else if (event->type() == QEvent::Show) {
			const QPoint pos = s.value("Processor/DialogPosition").toPoint();
			const QSize size = s.value("Processor/DialogSize").toSize();

			if (!pos.isNull())
				_processor->move(pos);

			if (!size.isNull())
				_processor->resize(size);

			_ui->actionProcessor->setChecked(true);
		}
	}

	return QMainWindow::eventFilter(object, event);
}


void MainWindow::abortScan() {
	forceAbortScan = true;
}


void MainWindow::toggleCddbSearchDialog(bool state) {
	if (state) {
		if (_cddbDialog)
			return;

		_cddbDialog = new CddbSearchDialog;

		_cddbDialog->setModel( _ui->tableItems->model() );
		_cddbDialog->setIndexes( _ui->tableItems->selectionModel()->selectedRows() );
		_cddbDialog->setWindowFlags(Qt::Window | Qt::Dialog | Qt::Popup);

		_cddbDialog->show();

		_ui->actionCDDB->setChecked(true);

		connect(this, SIGNAL(selectedRowsChanged(const QModelIndexList &)),
			_cddbDialog, SLOT(setIndexes(const QModelIndexList &)));

		connect(_cddbDialog, SIGNAL(finished(int)), SLOT(toggleCddbSearchDialog()));

	} else
		closeCddbDialog();
}

void MainWindow::openSettingsDialog() {
	SettingsDialog d(this);
	SettingsPage1 * p1 = new SettingsPage1(&d);
	SettingsPage2 * p2 = new SettingsPage2(&d);
	SettingsPage3 * p3 = new SettingsPage3(&d);
	SettingsPage4 * p4 = new SettingsPage4(&d);

	d.addPage(p1);
	d.addPage(p2);
	d.addPage(p3);
	d.addPage(p4);

	if (d.exec() == QDialog::Accepted) {
		QMap<QString, QVariant> data = d.settingsData();
		//_scanner->setRecursive(data.value("Scanning/ScanRecursive").toBool());
	}
}

void MainWindow::scanStarted() {
	findChild<QProgressBar*>("ScanProgressBar")->setVisible(true);
	findChild<QAction*>("actionAbort")->setEnabled(true);

	ImageCache::clear();
}

void MainWindow::scanFinished() {
	findChild<QProgressBar*>("ScanProgressBar")->setVisible(false);
	findChild<QAction*>("actionAbort")->setEnabled(false);
	findChild<QLabel*>("StatusMessage")->setText( QString("%1 tracks").arg(_dataModel->rowCount()) );
}

void MainWindow::setInterfaceLocked(bool setLocked) {
	foreach (QToolBar * o, findChildren<QToolBar*>())
		o->setMovable(!setLocked);

	foreach (QDockWidget * o, findChildren<QDockWidget*>()) {
		if (o->isFloating())
			continue;

		if (setLocked)
			o->setTitleBarWidget(new QWidget);

		else {
			if (QWidget * w = o->titleBarWidget())
				w->deleteLater();

			o->setTitleBarWidget(0);
		}
	}
}

void MainWindow::setInterfaceObjectVisible(bool setVisible) {
	if (QAction * a = qobject_cast<QAction*>(sender())) {
		if (QWidget * w = findChild<QWidget*>(a->data().toString()))
			w->setVisible(setVisible);
	}
}

void MainWindow::setRecursiveScan(bool state) {
	_scanner->setRecursive(state);
	_ui->actionRecursive_scan->setChecked(state);

	/*
	QSettings s;
	s.setValue("Scanning/ScanRecursive", state);
	*/
}





void MainWindow::loadSettings() {
	QSettings settings;

	const QString defaultHome = QDesktopServices::storageLocation(QDesktopServices::MusicLocation);
	const QString scannerHome = settings.value("Scanning/HomeDirectory", defaultHome).toString();
	bool recursive = settings.value("Scanning/ScanRecursive").toBool();

	_scanner->setRecursive(recursive);
	_scanner->setPath(scannerHome);

	_dataModel->setRootDirectory(scannerHome);

	resize( settings.value("MainWindow/Size", QSize(800, 400)).toSize() );
	restoreState( settings.value("MainWindow/State").toByteArray() );

	menuBar()->setVisible( settings.value("MainWindow/MenubarVisible", true).toBool() );

	const QList<QToolBar*> bars = findChildren<QToolBar*>();

	Qt::ToolButtonStyle style = static_cast<Qt::ToolButtonStyle>(
		settings.value("MainWindow/ToolBarButtonStyle", Qt::ToolButtonTextUnderIcon).toInt());

	int s = settings.value("MainWindow/ToolBarIconSize", 22).toInt();
	const QSize size(s, s);

	foreach (QToolBar * b, bars) {
		b->setIconSize(size);
		b->setToolButtonStyle(style);
	}

	_ui->widgetLocation->setPath(scannerHome);
	_ui->actionMenubar->setChecked( settings.value("MainWindow/MenubarVisible", true).toBool() );
	_ui->tableItems->horizontalHeader()->restoreState( settings.value("MainWindow/TableHeader").toByteArray() );
	_ui->actionProcessor->setChecked(settings.value("Processor/DockVisible", false).toBool());
	_ui->actionRecursive_scan->setChecked(settings.value("Scanning/ScanRecursive").toBool());

	_processor->setVisible(settings.value("Processor/DockVisible", false).toBool());

	setInterfaceLocked( settings.value("MainWindow/InterfaceLocked", false).toBool() );

	// This has to be here because in older version of Coquillo the sections weren't movable,
	// so that will be overridden by restoreState().
	_ui->tableItems->horizontalHeader()->setMovable(true);
}

void MainWindow::saveSettings() {
	QSettings settings;

	settings.beginGroup("MainWindow");
	settings.setValue("Size", size());
	// settings.setValue("Position", pos());
	settings.setValue("State", saveState());
	settings.setValue("TableHeader", _ui->tableItems->horizontalHeader()->saveState());
	settings.setValue("MenubarVisible", menuBar()->isVisible());

	if (QToolBar * b = findChildren<QToolBar*>().value(0)) {
		settings.setValue("InterfaceLocked", !b->isMovable());
		settings.setValue("ToolBarButtonStyle", b->toolButtonStyle());
		settings.setValue("ToolBarIconSize", b->iconSize().height());
	}

	settings.endGroup();

	settings.beginGroup("Processor");
	settings.setValue("DockVisible", _processor->isVisible());
	settings.endGroup();

	settings.sync();
}

void MainWindow::showAboutDialog() {
	AboutDialog d(this);
	d.exec();
}









void MainWindow::setToolBarIconSize(int size) {
	const QList<QToolBar*> bars = findChildren<QToolBar*>();

	foreach (QToolBar * tb, bars)
		tb->setIconSize(QSize(size,size));
}

void MainWindow::setToolBarButtonStyle(int style) {
	const QList<QToolBar*> bars = findChildren<QToolBar*>();

	foreach (QToolBar * tb, bars)
		tb->setToolButtonStyle(static_cast<Qt::ToolButtonStyle>(style)	);
}

void MainWindow::showToolBarContextMenu(const QPoint & pos) {
	QMenu menu;
	QMenu * popup = createPopupMenu();
	QMenu * sizeMenu = new QMenu(tr("Icon size"), &menu);
	QMenu * styleMenu = new QMenu(tr("Text position"), &menu);

	QAction * s = new QAction(tr("16x16"), sizeMenu);
	QAction * m = new QAction(tr("22x22"), sizeMenu);
	QAction * l = new QAction(tr("32x32"), sizeMenu);
	QAction * h = new QAction(tr("48x48"), sizeMenu);

	QAction * s1 = new QAction(tr("Icons only"), styleMenu);
	QAction * s2 = new QAction(tr("Text only"), styleMenu);
	QAction * s3 = new QAction(tr("Text alongside icons"), styleMenu);
	QAction * s4 = new QAction(tr("Text under icons"), styleMenu);

	s->setCheckable(true);
	m->setCheckable(true);
	l->setCheckable(true);
	h->setCheckable(true);

	s1->setCheckable(true);
	s2->setCheckable(true);
	s3->setCheckable(true);
	s4->setCheckable(true);

	int size = findChildren<QToolBar*>().at(0)->iconSize().height();
	int style = findChildren<QToolBar*>().at(0)->toolButtonStyle();

	switch (size) {
		case 16:
			s->setChecked(true);
			break;

		case 32:
			l->setChecked(true);
			break;

		case 48:
			h->setChecked(true);
			break;

		default:
			m->setChecked(true);
	}

	switch (style) {
		case Qt::ToolButtonTextOnly:
			s2->setChecked(true);
			break;

		case Qt::ToolButtonTextBesideIcon:
			s3->setChecked(true);
			break;

		case Qt::ToolButtonTextUnderIcon:
			s4->setChecked(true);
			break;

		default:
			s1->setChecked(true);
	}

	QActionGroup sizeGroup(this);
	sizeGroup.setExclusive(true);
	sizeGroup.addAction(s);
	sizeGroup.addAction(m);
	sizeGroup.addAction(l);
	sizeGroup.addAction(h);

	QActionGroup styleGroup(this);
	styleGroup.setExclusive(true);
	styleGroup.addAction(s1);
	styleGroup.addAction(s2);
	styleGroup.addAction(s3);
	styleGroup.addAction(s4);

	sizeMenu->addAction(s);
	sizeMenu->addAction(m);
	sizeMenu->addAction(l);
	sizeMenu->addAction(h);

	styleMenu->addAction(s1);
	styleMenu->addAction(s2);
	styleMenu->addAction(s3);
	styleMenu->addAction(s4);

	menu.addMenu(sizeMenu);
	menu.addMenu(styleMenu);
	menu.addSeparator();

	popup->addSeparator();
	popup->addMenu(sizeMenu);
	popup->addMenu(styleMenu);

	QSignalMapper sizeMapper(this);
	sizeMapper.setMapping(s, 16);
	sizeMapper.setMapping(m, 22);
	sizeMapper.setMapping(l, 32);
	sizeMapper.setMapping(h, 48);

	QSignalMapper styleMapper(this);
	styleMapper.setMapping(s1, Qt::ToolButtonIconOnly);
	styleMapper.setMapping(s2, Qt::ToolButtonTextOnly);
	styleMapper.setMapping(s3, Qt::ToolButtonTextBesideIcon);
	styleMapper.setMapping(s4, Qt::ToolButtonTextUnderIcon);

	connect(s, SIGNAL(triggered()), &sizeMapper, SLOT(map()));
	connect(m, SIGNAL(triggered()), &sizeMapper, SLOT(map()));
	connect(l, SIGNAL(triggered()), &sizeMapper, SLOT(map()));
	connect(h, SIGNAL(triggered()), &sizeMapper, SLOT(map()));

	connect(s1, SIGNAL(triggered()), &styleMapper, SLOT(map()));
	connect(s2, SIGNAL(triggered()), &styleMapper, SLOT(map()));
	connect(s3, SIGNAL(triggered()), &styleMapper, SLOT(map()));
	connect(s4, SIGNAL(triggered()), &styleMapper, SLOT(map()));

	connect(&sizeMapper, SIGNAL(mapped(int)), this, SLOT(setToolBarIconSize(int)));
	connect(&styleMapper, SIGNAL(mapped(int)), this, SLOT(setToolBarButtonStyle(int)));

	popup->exec(qobject_cast<QWidget*>(sender())->mapToGlobal(pos));

}

void MainWindow::showHeaderContextMenu(const QPoint & point) {
	QMenu menu(this);
	QHeaderView * header = qobject_cast<QHeaderView*>(sender());

	if (!header)
		return;

	QList<QAction*> actions;

	for (int i = 0; i < header->count(); i++) {
		QString label = header->model()->headerData(i, header->orientation()).toString();

		if (label == "Pictures")
			continue;
		else if (label == "MaxNumber")
			label = "Max Number";
		else if (label == "OriginalArtist")
			label = "Original Artist";

		label = tr( label.toAscii().constData() );

		QAction * a = menu.addAction( label );
		a->setCheckable(true);
		a->setChecked( !header->isSectionHidden(i) );

		actions << a;
	}

	QAction * action = menu.exec(header->mapToGlobal(point));

	if (action) {
		int i = actions.indexOf(action);
		header->setSectionHidden(i, !action->isChecked());
	}
}


void MainWindow::check() {
	qDebug() << "~()";
}




void MainWindow::saveMetaData() {
	QList<MetaData> metaData = _dataModel->modifiedMetaData(true);

	_writer->queue(metaData);
	_writer->write();

	_dataModel->saveChanges();

}

void MainWindow::closeCddbDialog() {
	if (!_cddbDialog)
		return;

	disconnect(_cddbDialog, SIGNAL(finished(int)), this, SLOT(toggleCddbSearchDialog()));

	_cddbDialog->close();
	_cddbDialog->deleteLater();
	_cddbDialog = 0;

	if (_ui->actionCDDB->isChecked())
		_ui->actionCDDB->setChecked(false);
}
