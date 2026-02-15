#include "mainwindow.h"
#include "filebrowser/filebrowser.h"
#include "mediastorage.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QSettings>
#include <QStandardPaths>
#include <QTimer>

namespace Coquillo {
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), storage(new MediaStorage(this)) {
    ui->setupUi(this);

    setup();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::setup() {
    auto fallbackLocation = QStandardPaths::standardLocations(QStandardPaths::MusicLocation).at(0);

    auto location = QSettings().value("DefaultLocation", fallbackLocation);

    ui->browser->setDirectory(location.toString());

    connect(ui->browser, &FileBrowser::pathAdded, storage, &MediaStorage::addPath);

    connect(ui->browser, &FileBrowser::pathRemoved, storage, &MediaStorage::removePath);

    connect(ui->browser, &FileBrowser::recursiveToggled, storage, &MediaStorage::setRecursive);

    connect(storage, &MediaStorage::started, [this]() {
        if (!progressBar) {
            progressBar = new QProgressBar;
            progressBar->setFixedWidth(260);
            progressBar->setRange(0, 0);
            ui->statusBar->addPermanentWidget(progressBar);
        }
    });

    connect(storage, &MediaStorage::finished, [this]() {
        if (progressBar) {
            ui->statusBar->removeWidget(progressBar);
            delete progressBar;
        }
    });

    connect(storage, &MediaStorage::progress, [this](int progress, int total) {
        if (progressBar) {
            progressBar->setRange(progress, total);
        }
    });

    QTimer::singleShot(1000, [this]() {
        storage->setRecursive(true);
        storage->addPath("/home/samu/Music");

        qDebug() << "SCAN INITIATED";
    });
}

} // namespace Coquillo
