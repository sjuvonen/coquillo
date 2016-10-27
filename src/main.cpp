
#include <QApplication>
#include "mainwindow.hpp"

int main(int argc, char ** args) {
    QApplication app(argc, args);
    Coquillo::MainWindow window;
    window.show();

    return app.exec();
}
