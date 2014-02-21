#include <QApplication>
#include "mainwindow.h"

int main(int argc, char ** args) {

    QApplication::setOrganizationName("Juvonet");
    QApplication::setOrganizationDomain("juvonet.fi");
    QApplication::setApplicationName("Coquillo");
    QApplication::setApplicationVersion("0.1");

    QApplication app(argc, args);
    Coquillo::MainWindow window;
    window.show();

    return app.exec();
}
