#include <QApplication>
#include <QDebug>
#include <QMap>
#include <QSettings>
#include <QStandardPaths>
#include "mainwindow.h"

void prepare_settings() {
    QSettings settings;
    QMap<QString, QVariant> defaults = {
        {"DefaultLocation", QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first()},
        {"RecursiveScan", true},
        {"DeleteEmptyDirs", true},
        {"FlacEnableId3v2", false},
        {"MpegEnableId3v1", false},
        {"Filter/SafeFilenames", false},
        {"Filter/ScalePictures", false},
        {"Filter/ParseDiscNumber", false},
    };

    foreach (const QString key, defaults.keys()) {
        if (!settings.contains(key)) {
            qDebug() << "set default" << key << defaults[key];
            settings.setValue(key, defaults[key]);
        }
    }

    settings.sync();
}

int main(int argc, char ** args) {
    QApplication::setOrganizationName("Juvonet");
    QApplication::setOrganizationDomain("juvonet.fi");
    QApplication::setApplicationName("Coquillo");
    QApplication::setApplicationVersion("1.9");

    QApplication app(argc, args);
    prepare_settings();
    Coquillo::MainWindow * window = new Coquillo::MainWindow;
    window->show();

    return app.exec();
}
