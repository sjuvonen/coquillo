#include <QApplication>
#include "mainwindow.h"

int main(int argc, char ** args) {
    QApplication::setOrganizationName("Juvonet");
    QApplication::setOrganizationDomain("juvonet.fi");
    QApplication::setApplicationName("Coquillo");
    QApplication::setApplicationVersion("1.9");

    QApplication app(argc, args);
    Coquillo::MainWindow * window = new Coquillo::MainWindow;
    window->show();

    return app.exec();
}
