
#include <QAction>
#include <QActionGroup>
#include <QDesktopServices>
#include <QDir>
#include <QFileSystemModel>
#include <QLabel>
#include <QMenuBar>
#include <QProgressBar>
#include <QSettings>
#include <QSignalMapper>
#include <QSortFilterProxyModel>
#include <QStatusBar>
#include <QThread>
#include <QTimer>
#include <QToolBar>

#include <metadata/ImageCache.h>
#include <metadata/MediaScanner.h>
#include <metadata/MetaDataModel.h>
#include <metadata/MetaDataWriter.h>

#include "AboutDialog.h"
#include "DirectorySelectorWidget.h"
#include "FileSystemProxyModel.h"
#include "MainWindow.h"
#include "ModelDataInspector.h"
#include "ProcessorWidget.h"

#include "SortFilterProxyModelMod.h"

#include "globals.h"

#include <cddb/CddbSearchDialog.h>
#include <bookmarks/BookmarkModel.h>
#include <bookmarks/BookmarkDialog.h>
#include <editor/EditorWidget.h>
#include <settings/SettingsDialog.h>
#include <settings/SettingsDialogPage.h>

#include "ui_MainWindow.h"


#include <QDebug>

volatile bool forceAbortScan = false;

MainWindow::MainWindow(QWidget * parent)
: QMainWindow(parent), _cddbDialog(0) {

	_ui = new Ui::MainWindow;
	_ui->setupUi(this);

	_bookmarkMapper = new QSignalMapper(this);
	_bookmarks = new BookmarkModel(this);
	
	_bookmarkProxy = new QSortFilterProxyModel(this);
	_bookmarkProxy->setSourceModel(_bookmarks);
	_bookmarkProxy->setDynamicSortFilter(true);
	_bookmarkProxy->sort(0);

	_processor = new ProcessorWidget(this);
	_processor->setWindowFlags( Qt::Window | Qt::Dialog | Qt::Popup );

	_inspector = new ModelDataInspector(this);

	_inspector->setModel( MetaDataModel::instance() );

	_scannerThread = new QThread(this);

	_writer = new MetaDataWriter;
	_writer->moveToThread(_scannerThread);

	_scannerThread->start();

	QSortFilterProxyModel * m = new SortFilterProxyModelMod;
	m->setDynamicSortFilter(true);
	m->setSourceModel( MetaDataModel::instance() );

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

	menuBar()->installEventFilter(this);
	statusBar()->installEventFilter(this);
	_processor->installEventFilter(this);
	_ui->mainToolBar->installEventFilter(this);


	// Add this action to MainWindow so that the shortcut works even when menubar is hidden.
	addAction( _ui->actionMenubar );
	addAction( _ui->actionQuit );
	addAction( _ui->actionConfigure );
	addAction( _ui->actionLock_interface );

	setupOldInterface();
	setupNewInterface();

	_ui->tableItems->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);


	connect(_bookmarkProxy, SIGNAL(rowsInserted(QModelIndex, int, int)),
		SLOT(setupBookmarksMenu()));
	
	connect(_bookmarkProxy, SIGNAL(rowsRemoved(QModelIndex, int, int)),
		SLOT(setupBookmarksMenu()));

	connect(_ui->tableItems->horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)),
		SLOT(showHeaderContextMenu(QPoint)));

	connect(_ui->widgetLocation, SIGNAL(pathSelected(QString)),
		MetaDataModel::instance(), SLOT(scan(QString)));

	connect(_ui->actionAbout, SIGNAL(triggered()), SLOT(showAboutDialog()));
	connect(_ui->actionConfigure, SIGNAL(triggered()), SLOT(openSettingsDialog()));
	connect(_ui->actionRecursive_scan, SIGNAL(toggled(bool)), SLOT(setRecursiveScan(bool)));

	connect(
		_ui->tableItems->selectionModel(),
		SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
  SLOT(slotSelectionChanged(QItemSelection))
	);

	connect(this, SIGNAL(selectedRowsChanged(QModelIndexList)),
		_ui->widgetEditor, SLOT(setDataIndexes(QModelIndexList)));

	connect(this, SIGNAL(selectedRowsChanged(QModelIndexList)),
		_processor, SLOT(setDataIndexes(const QModelIndexList)));

	connect(_processor, SIGNAL(closeButtonPressed()), _processor, SLOT(close()));

	connect(_ui->widgetLocation, SIGNAL(bookmarked(QString)),
		_bookmarks, SLOT(addPath(QString)));

	connect(
		_ui->widgetEditor->dataMapper(), SIGNAL(currentIndexChanged(int)),
		_ui->tableItems, SLOT(selectRow(int))
	);

	connect(_bookmarkMapper, SIGNAL(mapped(QString)),
		_ui->widgetLocation, SLOT(setPath(QString)));

	for (int i = 0; i < MetaDataModel::instance()->columnCount(); i++)
		if (i != modelColumn("Title") && i != modelColumn("Path") && i != modelColumn("Number"))
		_ui->tableItems->horizontalHeader()->hideSection(i);

	_ui->tableItems->horizontalHeader()->moveSection(modelColumn("Number"), 0);
	_ui->tableItems->horizontalHeader()->resizeSection(modelColumn("Number"), 35);
	_ui->tableItems->horizontalHeader()->resizeSection(0, 300);

	loadSettings();

	setupBookmarksMenu();
}


void MainWindow::openBookmarkDialog() {	
	BookmarkDialog d(this);
	d.setModel(_bookmarks);
	d.exec();
}

MainWindow::~MainWindow() {
	closeCddbDialog();
	delete _ui;
}

void MainWindow::slotSelectionChanged(const QItemSelection &) {
	emit selectedRowsChanged( _ui->tableItems->selectionModel()->selectedRows() );
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

	const QList<QToolBar *> bars = findChildren<QToolBar*>();

	m->addAction( _ui->actionLock_interface );
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

	return m;
}

void MainWindow::openDirectory(const QString & path) {
	_ui->widgetLocation->setRootPath(path);
	_ui->widgetLocation->setPath(path);

	MetaDataModel::instance()->setDirectory(path);
}


void MainWindow::setupBookmarksMenu() {
	_ui->menuBookmarks->clear();
	_ui->menuBookmarks->addAction(_ui->actionManage_bookmarks);
	_ui->menuBookmarks->addSeparator();

	for (int i = 0; i < _bookmarkProxy->rowCount(); i++) {
		const QModelIndex idx = _bookmarkProxy->index(i, 0);
		
		QAction * a = _ui->menuBookmarks->addAction(idx.data().toString());
		_bookmarkMapper->setMapping(a, idx.data(BookmarkModel::FilePathRole).toString());

		connect(a, SIGNAL(triggered()), _bookmarkMapper, SLOT(map()));
	}
}





void MainWindow::abortScan() {
	forceAbortScan = true;
}


void MainWindow::toggleCddbSearchDialog(bool state) {
	if (state) {
		if (_cddbDialog)
			return;

		_cddbDialog = new CddbSearchDialog(this);

		_cddbDialog->setModel( _ui->tableItems->model() );
		_cddbDialog->setIndexes( _ui->tableItems->selectionModel()->selectedRows() );
		_cddbDialog->setWindowFlags(Qt::Window | Qt::Dialog | Qt::Popup);

		_cddbDialog->show();

		_ui->actionCDDB->setChecked(true);
		_ui->buttonCDDB->setChecked(true);

		connect(this, SIGNAL(selectedRowsChanged(QModelIndexList)),
			_cddbDialog, SLOT(setIndexes(QModelIndexList)));

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

		qDebug() << "Check if scanner follows recursion rule!";
		//_scanner->setRecursive(data.value("Scanning/ScanRecursive").toBool());
	}
}

void MainWindow::scanStarted() {
	findChild<QProgressBar*>("ScanProgressBar")->setVisible(true);

	// Old interface
	_ui->actionReloadOrAbort->setText(tr("Abort"));
	_ui->actionReloadOrAbort->setIcon(style()->standardIcon(QStyle::SP_DialogCancelButton));
	_ui->actionReloadOrAbort->setShortcut(tr("Esc"));

	//disconnect(_ui->actionReloadOrAbort, SIGNAL(triggered()), _scanner, SLOT(scan()));
	connect(_ui->actionReloadOrAbort, SIGNAL(triggered()), SLOT(abortScan()));

	// New interface
	_ui->buttonReloadOrAbort->setText(tr("Abort"));
	_ui->buttonReloadOrAbort->setIcon(style()->standardIcon(QStyle::SP_DialogCancelButton));
	_ui->buttonReloadOrAbort->setShortcut(tr("Esc"));

	//disconnect(_ui->buttonReloadOrAbort, SIGNAL(clicked()), _scanner, SLOT(scan()));
	connect(_ui->buttonReloadOrAbort, SIGNAL(clicked()), SLOT(abortScan()));

	ImageCache::clear();
}

void MainWindow::scanFinished() {
	findChild<QProgressBar*>("ScanProgressBar")->setVisible(false);

	findChild<QLabel*>("StatusMessage")->setText(
		QString("%1 tracks").arg(MetaDataModel::instance()->rowCount())
	);

	// Old interface
	_ui->actionReloadOrAbort->setText(tr("Reload"));
	_ui->actionReloadOrAbort->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
	_ui->actionReloadOrAbort->setShortcut(tr("Ctrl+R"));

	disconnect(_ui->actionReloadOrAbort, SIGNAL(triggered()), this, SLOT(abortScan()));
	//connect(_ui->actionReloadOrAbort, SIGNAL(triggered()), _scanner, SLOT(scan()));

	// New interface
	_ui->buttonReloadOrAbort->setText(tr("Reload"));
	_ui->buttonReloadOrAbort->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
	_ui->buttonReloadOrAbort->setShortcut(tr("Ctrl+R"));

	disconnect(_ui->buttonReloadOrAbort, SIGNAL(clicked()), this, SLOT(abortScan()));
	//connect(_ui->buttonReloadOrAbort, SIGNAL(clicked()), _scanner, SLOT(scan()));
}

void MainWindow::setInterfaceLocked(bool setLocked) {

	_ui->actionLock_interface->setChecked(setLocked);

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
	_ui->actionRecursive_scan->setChecked(state);
	MetaDataModel::instance()->setRecursive(state);
}





void MainWindow::closeEvent(QCloseEvent * e) {
	abortScan();
	saveSettings();

	closeCddbDialog();

	QMainWindow::closeEvent(e);
}

bool MainWindow::eventFilter(QObject * object, QEvent * event) {

	if (object == _processor) {
		QSettings s;

		switch (event->type()) {
			case QEvent::Close:
			case QEvent::Hide:
				s.setValue("Processor/DialogPosition", _processor->pos());
				s.setValue("Processor/DialogSize", _processor->size());

				_ui->actionProcessor->setChecked(false);
				_ui->buttonProcessor->setChecked(false);
				return false;

			case QEvent::Show: {
				const QPoint pos = s.value("Processor/DialogPosition").toPoint();
				const QSize size = s.value("Processor/DialogSize").toSize();

				if (!pos.isNull())
					_processor->move(pos);

				if (!size.isNull())
					_processor->resize(size);

				_ui->actionProcessor->setChecked(true);
				return false;
			}

			default:
				return false;
		}
	} else if (object == menuBar()) {
		switch(event->type()) {
			case QEvent::Hide:
				_ui->buttonMenu->show();
				return false;

			case QEvent::Show:
				_ui->buttonMenu->hide();
				return false;

			default:
				return false;
		}
	} else if (object == _ui->mainToolBar) {
		switch(event->type()) {
			case QEvent::Hide:
				_ui->widgetToolbar->show();
				return false;

			case QEvent::Show:
				_ui->widgetToolbar->hide();
				return false;

			default:
				return false;
		}

	} else if (object == statusBar()) {
		switch(event->type()) {
			case QEvent::Hide:
				_ui->actionStatus_bar->setChecked(false);
				return false;

			case QEvent::Show:
				_ui->actionStatus_bar->setChecked(true);
				return false;

			default:
				return false;
		}
	}

	return QMainWindow::eventFilter(object, event);
}





void MainWindow::loadSettings() {
	QSettings settings;

	const QString defaultHome = QDesktopServices::storageLocation(QDesktopServices::MusicLocation);
	const QString scannerHome = settings.value("Scanning/HomeDirectory", defaultHome).toString();
	bool recursive = settings.value("Scanning/ScanRecursive").toBool();

	MetaDataModel::instance()->setRecursive(recursive);

	//MetaDataModel::instance()->setDirectory(scannerHome);

	resize( settings.value("MainWindow/Size", QSize(800, 400)).toSize() );
	restoreState( settings.value("MainWindow/State").toByteArray() );

	menuBar()->setVisible( settings.value("MainWindow/MenubarVisible", true).toBool() );
	statusBar()->setVisible( settings.value("MainWindow/StatusbarVisible", true).toBool() );

	const QList<QToolBar*> bars = findChildren<QToolBar*>();

	Qt::ToolButtonStyle style = static_cast<Qt::ToolButtonStyle>(
		settings.value("MainWindow/ToolBarButtonStyle", Qt::ToolButtonTextUnderIcon).toInt());

	int s = settings.value("MainWindow/ToolBarIconSize", 22).toInt();
	const QSize size(s, s);

	foreach (QToolBar * b, bars) {
		b->setIconSize(size);
		b->setToolButtonStyle(style);
	}

	_ui->widgetLocation->blockSignals(true);
	_ui->widgetLocation->setRootPath(scannerHome);
	_ui->widgetLocation->setPath(scannerHome);
	_ui->widgetLocation->blockSignals(false);

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
	settings.setValue("StatusbarVisible", statusBar()->isVisible());

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
	AboutDialog *d = new AboutDialog;

	d->exec();
	d->deleteLater();
}









void MainWindow::setToolBarIconSize(int size) {
	const QList<QToolBar*> bars = findChildren<QToolBar*>();
	const QList<QToolButton*> buttons = _ui->widgetToolbar->findChildren<QToolButton*>();

	foreach (QToolBar * tb, bars)
		tb->setIconSize(QSize(size,size));

	foreach (QToolButton * b, buttons)
		b->setIconSize(QSize(size, size));

}

void MainWindow::setToolBarButtonStyle(int style) {
	const QList<QToolBar*> bars = findChildren<QToolBar*>();
	const QList<QToolButton*> buttons = _ui->widgetToolbar->findChildren<QToolButton*>();

	foreach (QToolBar * tb, bars)
		tb->setToolButtonStyle(static_cast<Qt::ToolButtonStyle>(style));

	foreach (QToolButton * b, buttons)
		b->setToolButtonStyle(static_cast<Qt::ToolButtonStyle>(style));
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

	QActionGroup sizeGroup(&menu);
	sizeGroup.setExclusive(true);
	sizeGroup.addAction(s);
	sizeGroup.addAction(m);
	sizeGroup.addAction(l);
	sizeGroup.addAction(h);

	QActionGroup styleGroup(&menu);
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

	QSignalMapper sizeMapper(&menu);
	sizeMapper.setMapping(s, 16);
	sizeMapper.setMapping(m, 22);
	sizeMapper.setMapping(l, 32);
	sizeMapper.setMapping(h, 48);

	QSignalMapper styleMapper(&menu);
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

	popup->deleteLater();
}

void MainWindow::showHeaderContextMenu(const QPoint & point) {
	QMenu menu(this);
	QHeaderView * header = qobject_cast<QHeaderView*>(sender());

	if (!header)
		return;

	QList<QAction*> actions;

	for (int i = 0; i < header->count(); i++) {
		QString label = header->model()->headerData(i, header->orientation()).toString();

		/*
		if (label == "Pictures")
			continue;
		
		else
		*/

		if (label == "MaxNumber")
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

void MainWindow::closeCddbDialog() {
	if (!_cddbDialog)
		return;

	disconnect(_cddbDialog, SIGNAL(finished(int)), this, SLOT(toggleCddbSearchDialog()));

	_cddbDialog->close();
	_cddbDialog->deleteLater();
	_cddbDialog = 0;

	_ui->actionCDDB->setChecked(false);
	_ui->buttonCDDB->setChecked(false);
}



void MainWindow::setupNewInterface() {

	_ui->buttonPrevious->setIcon(style()->standardIcon(QStyle::SP_ArrowLeft));
	_ui->buttonNext->setIcon(style()->standardIcon(QStyle::SP_ArrowRight));
	_ui->buttonReset->setIcon(style()->standardIcon(QStyle::SP_DialogResetButton));
	_ui->buttonSave->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
	_ui->buttonReloadOrAbort->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
	_ui->buttonDirectory_up->setIcon(style()->standardIcon(QStyle::SP_ArrowUp));

	_ui->buttonCDDB->setIcon(style()->standardIcon(QStyle::SP_DriveCDIcon));
	_ui->buttonProcessor->setIcon(style()->standardIcon(QStyle::SP_ComputerIcon));

	MetaDataModel * mdm = MetaDataModel::instance();

	connect(_ui->buttonSave, SIGNAL(clicked()), mdm, SLOT(saveChanges()));
	connect(_ui->buttonReset, SIGNAL(clicked()), mdm, SLOT(undoChanges()));
	connect(_ui->buttonReloadOrAbort, SIGNAL(clicked()), mdm, SLOT(scan()));

	connect(mdm, SIGNAL(metaDataStateChanged(bool)), _ui->buttonSave, SLOT(setEnabled(bool)));
	connect(mdm, SIGNAL(metaDataStateChanged(bool)), _ui->buttonReset, SLOT(setEnabled(bool)));

	connect(
		_ui->buttonInverse, SIGNAL(clicked()),
		SLOT(slotSelectInverse())
	);

	connect(
		_ui->buttonNext, SIGNAL(clicked()),
		_ui->widgetEditor->dataMapper(), SLOT(toNext())
	);

	connect(
		_ui->buttonPrevious, SIGNAL(clicked()),
		_ui->widgetEditor->dataMapper(), SLOT(toPrevious())
	);

	connect(_ui->buttonCDDB, SIGNAL(toggled(bool)),
		SLOT(toggleCddbSearchDialog(bool)));

	connect(_ui->buttonProcessor, SIGNAL(toggled(bool)),
		_processor, SLOT(setVisible(bool)));

	connect(_ui->buttonDirectory_up, SIGNAL(clicked()),
		_ui->widgetLocation, SLOT(goDirectoryUp()));

	QMenu * menu = new QMenu;
	menu->addAction(_ui->actionMenubar);
	menu->addAction(_ui->actionStatus_bar);
	menu->addAction(_ui->actionLock_interface);
	menu->addSeparator();
	menu->addAction(_ui->actionRecursive_scan);
	menu->addSeparator();
	menu->addAction(_ui->actionConfigure);
	menu->addAction(_ui->actionAbout);
	menu->addAction(_ui->actionQuit);

	_ui->buttonMenu->setMenu(menu);
}


void MainWindow::setupOldInterface() {

	MetaDataModel * mdm = MetaDataModel::instance();

	connect(_ui->actionSave, SIGNAL(triggered()), mdm, SLOT(saveChanges()));
	connect(_ui->actionReset, SIGNAL(triggered()), mdm, SLOT(undoChanges()));
	connect(_ui->actionReloadOrAbort, SIGNAL(triggered()), mdm, SLOT(scan()));

	connect(mdm, SIGNAL(metaDataStateChanged(bool)), _ui->actionSave, SLOT(setEnabled(bool)));
	connect(mdm, SIGNAL(metaDataStateChanged(bool)), _ui->actionReset, SLOT(setEnabled(bool)));

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

	connect(_ui->actionCDDB, SIGNAL(toggled(bool)), SLOT(toggleCddbSearchDialog(bool)));
	connect(_ui->actionProcessor, SIGNAL(toggled(bool)), _processor, SLOT(setVisible(bool)));

	connect(_ui->actionDirectory_up, SIGNAL(triggered()),
		_ui->widgetLocation, SLOT(goDirectoryUp()));

	_ui->actionPrevious->setIcon(style()->standardIcon(QStyle::SP_ArrowLeft));
	_ui->actionNext->setIcon(style()->standardIcon(QStyle::SP_ArrowRight));
	_ui->actionReset->setIcon(style()->standardIcon(QStyle::SP_DialogResetButton));
	_ui->actionSave->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
	_ui->actionReloadOrAbort->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
	_ui->actionDirectory_up->setIcon(style()->standardIcon(QStyle::SP_ArrowUp));

	_ui->actionCDDB->setIcon(style()->standardIcon(QStyle::SP_DriveCDIcon));
	_ui->actionProcessor->setIcon(style()->standardIcon(QStyle::SP_ComputerIcon));

}
