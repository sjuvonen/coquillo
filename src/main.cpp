
#include <QApplication>
#include "mainwindow.hpp"

int main(int argc, char ** args) {
    QApplication::setOrganizationName("Juvonet");
    QApplication::setOrganizationDomain("juvonet.fi");
    QApplication::setApplicationName("Coquillo");
    QApplication::setApplicationVersion("1.9");
    
    QApplication app(argc, args);
    Coquillo::MainWindow window;
    window.show();

    return app.exec();
}
