
#include "tags/tagsmodel.hpp"
#include "mainwindow.hpp"
#include "ui_mainwindow.h"

namespace Coquillo {
    MainWindow::MainWindow(Qt::WindowFlags flags)
    : QMainWindow(0, flags) {
        _ui = new Ui::MainWindow;
        _ui->setupUi(this);

        auto model = new Tags::TagsModel(this);
        _ui->tableView->setModel(model);
    }

    MainWindow::~MainWindow() {
        delete _ui;
    }
}
