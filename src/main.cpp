#include <QApplication>
#include <QMap>
#include <QSettings>
#include <QStandardPaths>
#include "mainwindow.h"

void prepare_settings() {
    QSettings settings;
    QMap<QString, QVariant> defaults = {
        {"DefaultLocation", QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first()},
        {"RecursiveScan", true},
        {"SafeFilenames", true},
    };

    foreach (const QString key, defaults.keys()) {
        if (!settings.contains(key)) {
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
