#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSettings>
#include <QStandardPaths>

namespace Coquillo {
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  setup();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::setup() {
  auto fallbackLocation =
      QStandardPaths::standardLocations(QStandardPaths::MusicLocation).at(0);

  auto location = QSettings().value("DefaultLocation", fallbackLocation);

  ui->browser->setDirectory(location.toString());
}

} // namespace Coquillo
