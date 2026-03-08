#include "mainwindow.h"
#include "currentmediapathdelegate.h"
#include "filebrowser/filebrowser.h"
#include "mediastorage.h"
#include "mediastoragemodel.h"
#include "mediastoragemodelcolumns.h"
#include "selectionnotifier.h"
#include "settings.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QProgressBar>
#include <QSettings>
#include <QSignalMapper>
#include <QStandardPaths>
#include <QTimer>
#include <QWindowStateChangeEvent>
#include <qaction.h>
#include <qevent.h>
#include <qheaderview.h>
#include <qitemselectionmodel.h>
#include <qmainwindow.h>
#include <qmenubar.h>
#include <qnamespace.h>
#include <qpoint.h>
#include <qstatusbar.h>
#include <qtoolbar.h>

namespace Coquillo {
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), settings(new Settings(this)),
      storage(new MediaStorage(this)), storageModel(new MediaStorageModel(this)),
      proxyModel(new QSortFilterProxyModel(this)), selectionNotifier(new SelectionNotifier(this)),
      currentMediaPath(new CurrentMediaPathDelegate(this)) {
    ui->setupUi(this);

    storageModel->setStorage(storage);
    proxyModel->setSourceModel(storageModel);

    setup();
    restore();

    connect(ui->browser, &FileBrowser::pathAdded, storage, &MediaStorage::addPath);
    connect(ui->browser, &FileBrowser::pathRemoved, storage, &MediaStorage::removePath);
    connect(ui->browser, &FileBrowser::recursiveToggled, storage, &MediaStorage::setRecursive);

    connect(ui->actionLockToolBars, &QAction::toggled, [this](bool lock) { lockToolBars(lock); });

    connect(ui->actionMenubar, &QAction::toggled, ui->menuBar, &QMenuBar::setVisible);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionStatusbar, &QAction::toggled, ui->statusBar, &QStatusBar::setVisible);
    connect(ui->actionToolbar, &QAction::toggled, ui->toolBar, &QToolBar::setVisible);

    connect(ui->mediaView->horizontalHeader(), &QHeaderView::customContextMenuRequested, this,
            &MainWindow::showHeaderContextMenu);

    connect(ui->actionSelectAll, &QAction::triggered, [this]() {
        const auto first = proxyModel->index(0, 0);
        const auto last = first.siblingAtRow(proxyModel->rowCount() - 1);
        auto flags = QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows;

        ui->mediaView->selectionModel()->select(QItemSelection(first, last), flags);
    });

    QTimer::singleShot(1000, [this]() {
        storage->setRecursive(true);
        storage->addPath("/home/samu/Music/examples");

        qDebug() << "SCAN INITIATED";
    });
}

MainWindow::~MainWindow() { delete ui; }

QMenu *MainWindow::createPopupMenu() {
    auto menu = new QMenu(this);

    auto aMenuBar = new QAction(ui->actionMenubar->text());
    aMenuBar->setCheckable(true);
    aMenuBar->setChecked(ui->menuBar->isVisible());

    auto aToolBar = new QAction(ui->actionToolbar->text());
    aToolBar->setCheckable(true);
    aToolBar->setChecked(ui->toolBar->isVisible());

    auto aStatusBar = new QAction(ui->actionStatusbar->text());
    aStatusBar->setCheckable(true);
    aStatusBar->setChecked(ui->statusBar->isVisible());

    auto aLockBars = new QAction(ui->actionLockToolBars->text());
    aLockBars->setCheckable(true);
    aLockBars->setChecked(ui->actionLockToolBars->isChecked());

    menu->addAction(aMenuBar);
    menu->addAction(aStatusBar);
    menu->addSeparator();
    menu->addAction(aToolBar);
    menu->addSeparator();
    menu->addAction(aLockBars);

    connect(aMenuBar, &QAction::triggered, ui->menuBar, &QMenuBar::setVisible);
    connect(aToolBar, &QAction::triggered, ui->toolBar, &QToolBar::setVisible);
    connect(aStatusBar, &QAction::triggered, ui->statusBar, &QStatusBar::setVisible);
    connect(aLockBars, &QAction::triggered, this, &MainWindow::lockToolBars);

    return menu;
}

void MainWindow::setup() {

    addAction(ui->actionQuit);
    addAction(ui->actionSelectAll);
    addAction(ui->actionMenubar);

    auto progressBar = new QProgressBar(this);
    progressBar->setFixedWidth(220);
    progressBar->setRange(0, 0);
    progressBar->hide();

    ui->statusBar->addPermanentWidget(progressBar);

    ui->mediaView->setModel(proxyModel);
    ui->mediaView->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->mediaView->horizontalHeader()->setSectionsMovable(true);
    ui->mediaView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->mediaView->horizontalHeader()->resizeSection(0, 20);
    ui->mediaView->horizontalHeader()->resizeSection(1, 200);

    selectionNotifier->setModel(proxyModel);

    /**
     * NOTE: Must be called after `ui->mediaView->setModel`.
     */
    selectionNotifier->setSelectionModel(ui->mediaView->selectionModel());

    /**
     * NOTE: Must be called after `selection->setModel`.
     */
    ui->mediaTags->setSelectionNotifier(selectionNotifier);

    ui->mediaRaw->setMediaStorage(storage);
    ui->mediaRaw->setSelectionNotifier(selectionNotifier);

    ui->patternRename->setSelectionNotifier(selectionNotifier);
    ui->patternParse->setSelectionNotifier(selectionNotifier);

    currentMediaPath->setComboBox(ui->currentMediaPath);
    currentMediaPath->setSelectionNotifier(selectionNotifier);

    connect(storage, &MediaStorage::started, progressBar, &QProgressBar::show);
    connect(storage, &MediaStorage::finished, progressBar, &QProgressBar::hide);

    connect(storage, &MediaStorage::progress, [progressBar](int progress, int total) {
        progressBar->setMaximum(total);
        progressBar->setValue(progress);
    });
}

void MainWindow::restore() {
    const auto mediaViewHeaderState = settings->mediaViewHeaderState();
    const auto mainSplitterState = settings->mainSplitterState();
    const auto mainWindowState = settings->mainWindowState();

    resize(settings->mainWindowSize());
    lockToolBars(settings->toolBarsLocked());

    ui->menuBar->setVisible(settings->menuBarVisible());
    ui->statusBar->setVisible(settings->statusBarVisible());
    ui->browser->setDirectory(settings->location());

    if (settings->mainWindowMaximized()) {
        showMaximized();
    }

    if (!ui->mediaView->horizontalHeader()->restoreState(mediaViewHeaderState)) {
        for (int i = 0; i < ui->mediaView->horizontalHeader()->count(); i++) {
            if (i != MediaStorageModelColumns::indicatorColumn() &&
                i != MediaStorageModelColumns::pathColumn()) {
                ui->mediaView->hideColumn(i);
            }
        }
    }

    if (!ui->splitter->restoreState(mainSplitterState)) {
        ui->splitter->setStretchFactor(1, 10);
    }

    if (!restoreState(mainWindowState)) {
        // Initialize main window.
    }
}

void MainWindow::lockToolBars(bool lock) {
    ui->actionLockToolBars->setChecked(lock);

    for (auto bar : findChildren<QToolBar *>()) {
        bar->setMovable(!lock);
    }
}

void MainWindow::showHeaderContextMenu(const QPoint &pos) {
    QHeaderView *header = ui->mediaView->horizontalHeader();
    QSignalMapper mapper;
    QMap<QString, QAction *> labels;

    /*
     * Menu is created as a pointer to allow safe binding of the signal handler onto it.
     * That way we make sure that the listener is destroyed after it is not needed anymore.
     */
    QMenu *menu = new QMenu(header);

    QAction *action = menu->addAction(tr("Modification indicator"));
    action->setCheckable(true);
    action->setChecked(!header->isSectionHidden(0));
    menu->addSeparator();

    mapper.setMapping(action, 0);
    connect(action, SIGNAL(triggered()), &mapper, SLOT(map()));

    for (int i = 1; i < header->count(); i++) {
        const QString label = header->model()->headerData(i, Qt::Horizontal).toString();
        QAction *action = new QAction(label, menu);
        action->setCheckable(true);
        action->setChecked(!header->isSectionHidden(i));
        mapper.setMapping(action, i);
        labels[label] = action;

        connect(action, SIGNAL(triggered()), &mapper, SLOT(map()));
    }

    foreach (QAction *action, labels.values()) {
        menu->addAction(action);
    }

    void (QSignalMapper::*mapped_signal)(int) = &QSignalMapper::mappedInt;

    menu->connect(&mapper, mapped_signal, [=](int section) {
        header->setSectionHidden(section, !header->isSectionHidden(section));
    });

    menu->exec(header->mapToGlobal(pos));
    menu->deleteLater();
}

bool MainWindow::event(QEvent *event) {
    if (event->type() == QEvent::WindowStateChange) {
        const auto scEvent = dynamic_cast<QWindowStateChangeEvent *>(event);

        if (isMaximized() && !(scEvent->oldState() & Qt::WindowMaximized)) {
            settings->set("UI/Maximized", true);
        }

        if (!isMaximized() && scEvent->oldState() & Qt::WindowMaximized) {
            settings->set("UI/Maximized", false);
            resize(settings->mainWindowSize());
        }
    }

    if (event->type() == QEvent::Resize) {
        const auto rEvent = dynamic_cast<QResizeEvent *>(event);

        /**
         * NOTE: Need to delay check because maximize event will first trigger a regular resize.
         */
        QTimer::singleShot(10, [this]() {
            if (!isMaximized()) {
                settings->set("UI/Size", size());
            }
        });
    }

    return QMainWindow::event(event);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    // storage->abort();

    settings->set("UI/Header", ui->mediaView->horizontalHeader()->saveState());
    settings->set("UI/LockToolBars", ui->actionLockToolBars->isChecked());
    settings->set("UI/MenuBar", ui->actionMenubar->isChecked());
    settings->set("UI/Splitter", ui->splitter->saveState());
    settings->set("UI/State", saveState());
    settings->set("UI/StatusBar", ui->actionStatusbar->isChecked());

    QMainWindow::closeEvent(event);
}

} // namespace Coquillo
