#include "mainwindow.h"
#include "filebrowser/filebrowser.h"
#include "mediastorage.h"
#include "mediastoragemodel.h"
#include "settings.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QProgressBar>
#include <QSettings>
#include <QStandardPaths>
#include <QTimer>
#include <qprogressbar.h>

namespace Coquillo {
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), settings(new Settings(this)),
      storage(new MediaStorage(this)), model(new MediaStorageModel(this)) {
    ui->setupUi(this);

    model->setStorage(storage);

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
    ui->mediaView->setModel(model);

    connect(ui->browser, &FileBrowser::pathAdded, storage, &MediaStorage::addPath);
    connect(ui->browser, &FileBrowser::pathRemoved, storage, &MediaStorage::removePath);
    connect(ui->browser, &FileBrowser::recursiveToggled, storage, &MediaStorage::setRecursive);

    connect(storage, &MediaStorage::started, progressBar, &QProgressBar::show);
    connect(storage, &MediaStorage::finished, progressBar, &QProgressBar::hide);
    connect(storage, &MediaStorage::progress, [progressBar](int progress, int total) {
        progressBar->setMaximum(total);
        progressBar->setValue(progress);
    });
}
void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);

    settings->set("UI/Size", size());
}

} // namespace Coquillo
