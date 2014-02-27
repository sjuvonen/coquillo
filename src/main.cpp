#include <QApplication>
#include "mainwindow.h"

#include <QDebug>
#include <QVariantHash>
#include <processor/patterns.h>

int main(int argc, char ** args) {
    QApplication::setOrganizationName("Juvonet");
    QApplication::setOrganizationDomain("juvonet.fi");
    QApplication::setApplicationName("Coquillo");
    QApplication::setApplicationVersion("0.1");

    QApplication app(argc, args);
    Coquillo::MainWindow window;
    window.show();
    return app.exec();

//     QVariantHash values;
//     values["artist"] = "Scooter";
//     values["album"] = "24 Carat Gold";
//     values["disc"] = 1;
//     values["number"] = 1;
//     values["title"] = "Nessaja";
//     values["genre"] = "Trance";
//
//     Coquillo::Processor::Patterns patterns;
//     qDebug() << patterns.compile("%b/Disc %d/%a - %t %%d", values);

//     Coquillo::Processor::Patterns patterns;
//     QString path = "24 Carat Gold/Disc 1/Scooter - Nessaja";
//     QString pattern = "%b/Disc %d/%a - %t";
//     QVariantHash tags = patterns.extract(pattern, path);
//     qDebug() << tags;
}
