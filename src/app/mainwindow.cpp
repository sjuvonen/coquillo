#include "mainwindow.h"
#include "currentmediapathdelegate.h"
#include "filebrowser/filebrowser.h"
#include "mediastorage.h"
#include "mediastoragemodel.h"
#include "selectionnotifier.h"
#include "settings.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QProgressBar>
#include <QSettings>
#include <QStandardPaths>
#include <QTimer>
#include <QWindowStateChangeEvent>
#include <qaction.h>
#include <qitemselectionmodel.h>
#include <qmenubar.h>
#include <qnamespace.h>
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

    menu->addAction(aMenuBar);
    menu->addAction(aStatusBar);
    menu->addSeparator();
    menu->addAction(aToolBar);

    connect(aMenuBar, &QAction::triggered, ui->menuBar, &QMenuBar::setVisible);
    connect(aToolBar, &QAction::triggered, ui->toolBar, &QToolBar::setVisible);
    connect(aStatusBar, &QAction::triggered, ui->statusBar, &QStatusBar::setVisible);

    return menu;
}

void MainWindow::setup() {
    auto fallbackLocation = QStandardPaths::standardLocations(QStandardPaths::MusicLocation).at(0);
    auto location = QSettings().value("DefaultLocation", fallbackLocation);

    auto progressBar = new QProgressBar(this);
    progressBar->setFixedWidth(220);
    progressBar->setRange(0, 0);
    progressBar->hide();

    ui->statusBar->addPermanentWidget(progressBar);
    ui->browser->setDirectory(location.toString());

    ui->mediaView->setModel(proxyModel);
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

    currentMediaPath->setComboBox(ui->currentMediaPath);
    currentMediaPath->setSelectionNotifier(selectionNotifier);

    connect(ui->browser, &FileBrowser::pathAdded, storage, &MediaStorage::addPath);
    connect(ui->browser, &FileBrowser::pathRemoved, storage, &MediaStorage::removePath);
    connect(ui->browser, &FileBrowser::recursiveToggled, storage, &MediaStorage::setRecursive);

    connect(ui->actionSelectAll, &QAction::triggered, [this]() {
        const auto first = proxyModel->index(0, 0);
        const auto last = first.siblingAtRow(proxyModel->rowCount() - 1);
        auto flags = QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows;

        ui->mediaView->selectionModel()->select(QItemSelection(first, last), flags);
    });

    connect(storage, &MediaStorage::started, progressBar, &QProgressBar::show);
    connect(storage, &MediaStorage::finished, progressBar, &QProgressBar::hide);

    connect(storage, &MediaStorage::progress, [progressBar](int progress, int total) {
        progressBar->setMaximum(total);
        progressBar->setValue(progress);
    });

    QTimer::singleShot(1000, [this]() {
        storage->setRecursive(true);
        storage->addPath("/home/samu/Music/examples");

        qDebug() << "SCAN INITIATED";
    });

    addAction(ui->actionQuit);
    addAction(ui->actionSelectAll);
    addAction(ui->actionMenubar);

    resize(settings->mainWindowSize());

    if (settings->mainWindowMaximized()) {
        showMaximized();
    }
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

} // namespace Coquillo
