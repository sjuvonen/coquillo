
#include <QDebug>

#include <QProgressBar>
#include <QSettings>
#include <QSignalMapper>
#include <QSortFilterProxyModel>

#include <core/MetaDataModel2.h>

#include "AboutDialog.h"
#include "BookmarkDialog.h"
#include "BookmarkModel.h"
#include "CddbSearchDialog.h"
#include "MainWindow.h"
#include "ProcessorWidget.h"
#include "SettingsDialog.h"

#include "ui_MainWindow.h"

MainWindow::MainWindow(MetaDataModel * model, QWidget * parent)
: QMainWindow(parent), _model(0) {

	_ui = new Ui::MainWindow();
	_ui->setupUi(this);

	QSortFilterProxyModel * proxy = new QSortFilterProxyModel(this);
	proxy->setDynamicSortFilter(true);
	
	_ui->editor->setModel(proxy);
	_ui->items->setModel(proxy);

	_ui->items->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);

	_cddbSearch = new CddbSearchDialog(this);
	_cddbSearch->setWindowFlags(Qt::Tool);
	_cddbSearch->hide();

	_cddbSearch->installEventFilter(this);
	_cddbSearch->setModel(proxy);
	
	_processor = new ProcessorWidget(this);
	_processor->setWindowFlags(Qt::Tool);
	_processor->hide();

	_processor->installEventFilter(this);
	_processor->setModel(proxy);

	findChild<QToolBar*>()->installEventFilter(this);

	_bookmarks = new BookmarkModel(this);

	setMetaDataModel(model);
// 	loadSettings();

	proxy->sort(
		_ui->items->horizontalHeader()->sortIndicatorSection(),
		_ui->items->horizontalHeader()->sortIndicatorOrder()
	);

	setupBookmarkMenu();
	setupToolBar();

	addAction(_ui->actionToggleMenuBar);
	addAction(_ui->actionQuit);

	model->setDirectory(_ui->location->path());

	connect(_ui->items->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
		SLOT(proxyTableSelection()));

	connect(_ui->items->horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)),
		SLOT(showHeaderContextMenu(QPoint)));

	connect(_ui->actionToggleProcessor, SIGNAL(toggled(bool)),
		_processor, SLOT(setVisible(bool)));

	connect(_ui->actionToggleCddb, SIGNAL(toggled(bool)),
		_cddbSearch, SLOT(setVisible(bool)));

	connect(_ui->location, SIGNAL(bookmarked(QString)),
		_bookmarks, SLOT(addPath(QString)));

	connect(_bookmarks, SIGNAL(dataChanged(QModelIndex, QModelIndex)),
		SLOT(setupBookmarkMenu()));

	connect(_bookmarks, SIGNAL(rowsInserted(QModelIndex, int, int)),
		SLOT(setupBookmarkMenu()));

	connect(_bookmarks, SIGNAL(rowsRemoved(QModelIndex, int, int)),
		SLOT(setupBookmarkMenu()));

	foreach (QDockWidget * dock, dockWidgets())
		dock->installEventFilter(this);
}

MainWindow::~MainWindow() {
	delete _ui;
}

QMenu * MainWindow::createPopupMenu() {

	QMenu * menu = new QMenu(tr("Main Menu"), this);

	QAction * lock = menu->addAction(tr("&Lock interface"));
	lock->setCheckable(true);
	lock->setChecked(!findChild<QToolBar*>()->isMovable());

	QAction * mainMenu = menu->addAction(tr("Main menu"));
	mainMenu->setCheckable(true);
	mainMenu->setChecked(menuBar()->isVisible());

	foreach (QToolBar * bar, findChildren<QToolBar*>()) {
		QAction * a = menu->addAction(bar->windowTitle());
		a->setCheckable(true);
		a->setChecked(bar->isVisible());

		connect(a, SIGNAL(triggered(bool)), bar, SLOT(setVisible(bool)));
	}

	menu->addSeparator();

	foreach (QDockWidget * dock, findChildren<QDockWidget*>()) {
		QAction * a = menu->addAction(dock->windowTitle());
		a->setCheckable(true);
		a->setChecked(dock->isVisible());
		
		connect(a, SIGNAL(triggered(bool)), dock, SLOT(setVisible(bool)));
	}
	QMenu * sizeMenu = new QMenu(tr("Icon size"), menu);
	QMenu * styleMenu = new QMenu(tr("Text position"), menu);

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

	QActionGroup * sizeGroup = new QActionGroup(menu);
	sizeGroup->setExclusive(true);
	sizeGroup->addAction(s);
	sizeGroup->addAction(m);
	sizeGroup->addAction(l);
	sizeGroup->addAction(h);

	QActionGroup * styleGroup = new QActionGroup(menu);
	styleGroup->setExclusive(true);
	styleGroup->addAction(s1);
	styleGroup->addAction(s2);
	styleGroup->addAction(s3);
	styleGroup->addAction(s4);

	sizeMenu->addAction(s);
	sizeMenu->addAction(m);
	sizeMenu->addAction(l);
	sizeMenu->addAction(h);

	styleMenu->addAction(s1);
	styleMenu->addAction(s2);
	styleMenu->addAction(s3);
	styleMenu->addAction(s4);

	menu->addSeparator();
	menu->addMenu(sizeMenu);
	menu->addMenu(styleMenu);
	menu->addSeparator();

	QSignalMapper * sizeMapper = new QSignalMapper(menu);
	sizeMapper->setMapping(s, 16);
	sizeMapper->setMapping(m, 22);
	sizeMapper->setMapping(l, 32);
	sizeMapper->setMapping(h, 48);

	QSignalMapper * styleMapper = new QSignalMapper(menu);
	styleMapper->setMapping(s1, Qt::ToolButtonIconOnly);
	styleMapper->setMapping(s2, Qt::ToolButtonTextOnly);
	styleMapper->setMapping(s3, Qt::ToolButtonTextBesideIcon);
	styleMapper->setMapping(s4, Qt::ToolButtonTextUnderIcon);

	connect(s, SIGNAL(triggered()), sizeMapper, SLOT(map()));
	connect(m, SIGNAL(triggered()), sizeMapper, SLOT(map()));
	connect(l, SIGNAL(triggered()), sizeMapper, SLOT(map()));
	connect(h, SIGNAL(triggered()), sizeMapper, SLOT(map()));

	connect(s1, SIGNAL(triggered()), styleMapper, SLOT(map()));
	connect(s2, SIGNAL(triggered()), styleMapper, SLOT(map()));
	connect(s3, SIGNAL(triggered()), styleMapper, SLOT(map()));
	connect(s4, SIGNAL(triggered()), styleMapper, SLOT(map()));

	connect(sizeMapper, SIGNAL(mapped(int)),
		SLOT(setToolBarIconSize(int)));

	connect(styleMapper, SIGNAL(mapped(int)),
		SLOT(setToolBarButtonStyle(int)));










	
	
	connect(mainMenu, SIGNAL(triggered(bool)),
		menuBar(), SLOT(setVisible(bool)));
	
	connect(lock, SIGNAL(triggered(bool)),
		SLOT(setInterfaceLocked(bool)));

	return menu;
}

QList<QToolBar *> MainWindow::toolBars() const {
	return findChildren<QToolBar*>();
}

QList<QDockWidget *> MainWindow::dockWidgets() const {
	return findChildren<QDockWidget*>();
}

bool MainWindow::eventFilter(QObject * object, QEvent * event) {

	if (event->type() == QEvent::Hide || event->type() == QEvent::Show) {
		if (object == _processor)
			_ui->actionToggleProcessor->setChecked(_processor->isVisible());

		else if (object == _cddbSearch)
			_ui->actionToggleCddb->setChecked(_cddbSearch->isVisible());

		else if (object == _ui->dockLocation)
			_ui->actionToggleLocationDock->setChecked(_ui->dockLocation->isVisible());

		else if (object == _ui->dockEditor)
			_ui->actionToggleEditorDock->setChecked(_ui->dockEditor->isVisible());

		else if (object->inherits("QToolBar"))
			_ui->actionToggleMainToolBar->setChecked(findChild<QToolBar*>()->isVisible());
	}

	return QMainWindow::eventFilter(object, event);
}




/**
 * PUBLIC SLOTS
 **/

void MainWindow::closeEvent(QCloseEvent *) {	
	QSettings s;

	s.beginGroup("Widgets");
	
	s.setValue("ItemsTableHeaderState", _ui->items->horizontalHeader()->saveState());
	s.setValue("MainWindowState", saveState());
	s.setValue("MainWindowSize", size());
	s.setValue("MenuBarHidden", menuBar()->isHidden());
	s.setValue("StatusBarHidden", statusBar()->isHidden());
	
	if (QToolBar * b = toolBars().value(0)) {
		s.setValue("MainWindowLocked", !b->isMovable());
		s.setValue("ToolBarButtonStyle", b->toolButtonStyle());
		s.setValue("ToolBarIconSize", b->iconSize().height());
	}
}

void MainWindow::setInterfaceLocked(bool state) {
	foreach (QToolBar * bar, toolBars())
		bar->setMovable(!state);

	foreach (QDockWidget * dock, dockWidgets()) {
		if (state)
			dock->setTitleBarWidget(new QWidget(dock));
		else {
			if (QWidget * w = dock->titleBarWidget())
				w->deleteLater();

			dock->setTitleBarWidget(0);
		}
	}
}

void MainWindow::setToolBarIconSize(int size) {
	const QList<QToolBar*> bars = findChildren<QToolBar*>();
	const QList<QToolButton*> buttons = _ui->mainToolBar->findChildren<QToolButton*>();

	foreach (QToolBar * tb, bars)
		tb->setIconSize(QSize(size,size));

	foreach (QToolButton * b, buttons)
		b->setIconSize(QSize(size, size));

}

void MainWindow::setToolBarButtonStyle(int style) {
	const QList<QToolBar*> bars = findChildren<QToolBar*>();
	const QList<QToolButton*> buttons = _ui->mainToolBar->findChildren<QToolButton*>();

	foreach (QToolBar * tb, bars)
		tb->setToolButtonStyle(static_cast<Qt::ToolButtonStyle>(style));

	foreach (QToolButton * b, buttons)
		b->setToolButtonStyle(static_cast<Qt::ToolButtonStyle>(style));
}





/**
 * PROTECTED
 **/

void MainWindow::showEvent(QShowEvent * event) {
	loadSettings();
	
	QMainWindow::showEvent(event);
	QSettings s;

	_ui->actionToggleLocationDock->setChecked(_ui->dockLocation->isVisible());
	_ui->actionToggleEditorDock->setChecked(_ui->dockEditor->isVisible());

	_ui->actionToggleMainToolBar->setChecked(findChild<QToolBar*>()->isVisible());

	_ui->actionToggleMenuBar->setChecked(menuBar()->isVisible());
	_ui->actionToggleStatusBar->setChecked(statusBar()->isVisible());
}





/**
 * PRIVATE SLOTS
 **/

void MainWindow::proxyTableSelection() {
	_ui->editor->setSelection(_ui->items->selectionModel()->selection());
	_cddbSearch->setSelection(_ui->items->selectionModel()->selection());
	_processor->setSelection(_ui->items->selectionModel()->selection());
}

void MainWindow::selectInverse() {
	QAbstractItemModel * m = _ui->items->model();

	const QModelIndex tl = m->index(0, 0);
	const QModelIndex br = m->index(m->rowCount()-1, m->columnCount()-1);
	const QItemSelection s(tl, br);

	_ui->items->selectionModel()->select(s, QItemSelectionModel::Toggle);
}

void MainWindow::selectNextIndex() {
	const QModelIndex idx = _ui->items->currentIndex();

	if (idx.row() < idx.model()->rowCount() - 1)
		_ui->items->setCurrentIndex(idx.sibling(idx.row()+1, 0));
}

void MainWindow::selectPreviousIndex() {
	const QModelIndex idx = _ui->items->currentIndex();

	if (idx.row() > 0)
		_ui->items->setCurrentIndex(idx.sibling(idx.row()-1, 0));
	
}

void MainWindow::setupBookmarkMenu() {
	_ui->menuBookmarks->clear();
	_ui->menuBookmarks->addAction(_ui->actionManage_bookmarks);
	_ui->menuBookmarks->addSeparator();

	QSignalMapper * mapper = findChild<QSignalMapper*>("BookmarkMapper");

	if (!mapper) {
		mapper = new QSignalMapper(this);
		mapper->setObjectName("BookmarkMapper");

		connect(mapper, SIGNAL(mapped(QString)),
			_ui->location, SLOT(setPath(QString)));
	}

	QMap<QString, QString> data;

	for (int i = 0; i < _bookmarks->rowCount(); i++) {
		const QModelIndex idx = _bookmarks->index(i, 0);
		data.insert(idx.data().toString(), idx.data(BookmarkModel::FilePathRole).toString());
	}

	foreach (const QString path, data.keys()) {
		QAction * a = _ui->menuBookmarks->addAction(path);
		mapper->setMapping(a, data[path]);

		connect(a, SIGNAL(triggered()), mapper, SLOT(map()));
	}
}

void MainWindow::showAboutDialog() {
	AboutDialog d(this);
	d.exec();
}

void MainWindow::showBookmarkDialog() {
	BookmarkDialog d(this);
	d.setModel(_bookmarks);
	d.exec();
}

void MainWindow::showSettingsDialog() {
	QSettings s;
	
	SettingsDialog d(this);
	d.setBackend(&s);
	d.exec();
}

void MainWindow::showHeaderContextMenu(const QPoint & pos) {

	QHeaderView * header = qobject_cast<QHeaderView*>(sender());

	if (!header)
		return;
	
	QMenu menu(header);
	QMap<QString, int> columns;

	for (int i = 0; i < header->count(); i++)
		columns.insert(header->model()->headerData(i, Qt::Horizontal).toString(), i);

	foreach (const QString label, columns.keys()) {
		int i = columns[label];
		
		QAction * a = menu.addAction(label);
		a->setCheckable(true);
		a->setChecked(!header->isSectionHidden(i));
	}

	QAction * selected = menu.exec(header->mapToGlobal(pos));

	if (selected) {
		int col = columns[selected->text()];
		header->setSectionHidden(col, !header->isSectionHidden(col));
	}
}

void MainWindow::afterModelAction() {
	_ui->actionReloadOrAbort->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
	_ui->actionReloadOrAbort->setText(tr("Reload"));

	/*
	if (QSortFilterProxyModel * p = qobject_cast<QSortFilterProxyModel*>(_ui->items->model()))
		p->setDynamicSortFilter(false);
	*/

	disconnect(_ui->actionReloadOrAbort, SIGNAL(triggered()),
		_model, SLOT(abortAction()));

	connect(_ui->actionReloadOrAbort, SIGNAL(triggered()),
		_model, SLOT(reload()));
}

void MainWindow::beforeModelAction() {
	QProgressBar * bar = new QProgressBar(this);

	bar->setMaximumWidth(120);
	bar->setMaximumHeight(statusBar()->size().height() - 5);
	
	statusBar()->addWidget(bar);

	_ui->actionReloadOrAbort->setIcon(style()->standardIcon(QStyle::SP_BrowserStop));
	_ui->actionReloadOrAbort->setText(tr("Abort"));

	/*
	if (QSortFilterProxyModel * p = qobject_cast<QSortFilterProxyModel*>(_ui->items->model()))
		p->setDynamicSortFilter(true);
	*/

	connect(_model, SIGNAL(actionStarted()), bar, SLOT(show()));
	connect(_model, SIGNAL(actionFinished()), bar, SLOT(hide()));
	connect(_model, SIGNAL(actionFinished()), bar, SLOT(deleteLater()));

	connect(_model, SIGNAL(actionProgress(int)), bar, SLOT(setValue(int)));
	connect(_model, SIGNAL(actionMaximumChanged(int)), bar, SLOT(setMaximum(int)));

	disconnect(_ui->actionReloadOrAbort, SIGNAL(triggered()),
		_model, SLOT(reload()));

	connect(_ui->actionReloadOrAbort, SIGNAL(triggered()),
		_model, SLOT(abortAction()));
}





/**
 * PRIVATE METHODS
 **/

void MainWindow::loadSettings() {
	/**
	 * Note that some of the settings are restored in showEvent() due to
	 * the fact that widgets return false from isVisible() before the main window
	 * has been made visible!
	 **/
	QSettings s;

	_ui->location->blockSignals(true);
	_ui->location->setRootPath(_model->directory());
	_ui->location->blockSignals(false);

	if (s.contains("Widgets/ItemsTableHeaderState"))
		_ui->items->horizontalHeader()->restoreState(s.value("Widgets/ItemsTableHeaderState").toByteArray());
	else {
		for (int i = MetaData::TitleField; i <= MetaData::PicturesField; i++) {
			if (i != MetaData::PathField)
				_ui->items->horizontalHeader()->setSectionHidden(i, true);
		}
	}

	if (s.contains("Widgets/MainWindowSize")) {
		restoreState(s.value("Widgets/MainWindowState").toByteArray());
		resize(s.value("Widgets/MainWindowSize").toSize());
	} else
		resize(QSize(600, 480));

	setToolBarIconSize(s.value("Widgets/ToolBarIconSize").toInt());
	setToolBarButtonStyle(s.value("Widgets/ToolBarButtonStyle").toInt());
	setInterfaceLocked(s.value("Widgets/MainWindowLocked").toBool());
	
	menuBar()->setHidden(s.value("Widgets/MenuBarHidden").toBool());
	statusBar()->setHidden(s.value("Widgets/StatusBarHidden").toBool());
}

void MainWindow::setMetaDataModel(MetaDataModel * model) {
	if (model) {
		connect(_ui->actionSave, SIGNAL(triggered()),
			model, SLOT(save()));

		connect(_ui->actionReset, SIGNAL(triggered()),
			model, SLOT(restore()));
		connect(_ui->actionReloadOrAbort, SIGNAL(triggered()),
			model, SLOT(reload()));

		connect(_ui->actionRecursiveScan, SIGNAL(toggled(bool)),
			model, SLOT(setRecursive(bool)));

		connect(_ui->location, SIGNAL(pathChanged(QString)),
			model, SLOT(readDirectory(QString)));

		connect(model, SIGNAL(metaDataStateChanged(bool)),
			_ui->actionSave, SLOT(setEnabled(bool)));

		connect(model, SIGNAL(metaDataStateChanged(bool)),
			_ui->actionReset, SLOT(setEnabled(bool)));

		connect(model, SIGNAL(actionStarted()),
			SLOT(beforeModelAction()));

		connect(model, SIGNAL(actionFinished()),
			SLOT(afterModelAction()));
	}

	if (_model) {

	}

	_model = model;

	qobject_cast<QSortFilterProxyModel*>(_ui->items->model())->setSourceModel(model);
}

void MainWindow::setupToolBar() {

	_ui->actionPrevious->setIcon(style()->standardIcon(QStyle::SP_ArrowLeft));
	_ui->actionNext->setIcon(style()->standardIcon(QStyle::SP_ArrowRight));
	_ui->actionReset->setIcon(style()->standardIcon(QStyle::SP_DialogResetButton));
	_ui->actionSave->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
	_ui->actionReloadOrAbort->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
	_ui->actionLevel_up->setIcon(style()->standardIcon(QStyle::SP_ArrowUp));

	_ui->actionToggleCddb->setIcon(style()->standardIcon(QStyle::SP_DriveCDIcon));
	_ui->actionToggleProcessor->setIcon(style()->standardIcon(QStyle::SP_ComputerIcon));

	connect(_ui->actionLevel_up, SIGNAL(triggered()),
		_ui->location, SLOT(goDirectoryUp()));

	_ui->actionSave->setShortcut(QKeySequence::Save);
	_ui->actionSelect_all->setShortcut(QKeySequence::SelectAll);
	_ui->actionReloadOrAbort->setShortcut(QKeySequence::Refresh);
	_ui->actionNext->setShortcut(QKeySequence::MoveToNextPage);
	_ui->actionPrevious->setShortcut(QKeySequence::MoveToPreviousPage);
	_ui->actionQuit->setShortcut(QKeySequence::Quit);

	// This mapping seems to be empty at least on Linux.
	// Fallback value is Ctrl+P which is bound in the QtDesigner .ui file.
	if (!QKeySequence::keyBindings(QKeySequence::Preferences).isEmpty())
		_ui->actionConfigure->setShortcut(QKeySequence::Preferences);
	
}
