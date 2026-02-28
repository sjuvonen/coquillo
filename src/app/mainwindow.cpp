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
#include <qnamespace.h>

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

    selectionNotifier->setModel(proxyModel);

    /**
     * NOTE: Must be called after `ui->mediaView->setModel`.
     */
    selectionNotifier->setSelectionModel(ui->mediaView->selectionModel());

    /**
     * NOTE: Must be called after `selection->setModel`.
     */
    ui->mediaTags->setSelectionNotifier(selectionNotifier);

    currentMediaPath->setComboBox(ui->currentMediaPath);
    currentMediaPath->setSelectionNotifier(selectionNotifier);

    connect(ui->browser, &FileBrowser::pathAdded, storage, &MediaStorage::addPath);
    connect(ui->browser, &FileBrowser::pathRemoved, storage, &MediaStorage::removePath);
    connect(ui->browser, &FileBrowser::recursiveToggled, storage, &MediaStorage::setRecursive);

    connect(storage, &MediaStorage::started, progressBar, &QProgressBar::show);
    connect(storage, &MediaStorage::finished, progressBar, &QProgressBar::hide);

    connect(storage, &MediaStorage::progress, [progressBar](int progress, int total) {
        progressBar->setMaximum(total);
        progressBar->setValue(progress);
    });

    QTimer::singleShot(1000, [this]() {
        storage->setRecursive(true);
        storage->addPath("/home/samu/Music");

        qDebug() << "SCAN INITIATED";
    });

    addAction(ui->actionQuit);

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
