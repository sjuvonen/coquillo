#include <QApplication>
#include <QSettings>
#include <QStandardPaths>
#include <QTranslator>

#include "mainwindow.hpp"

void prepare_settings() {
    const QVariantMap defaults = {
        {"DefaultLocation", QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first()},
        {"DeleteEmptyDirs", true},
        {"NumberPadWidth", 2},
        {"RecursiveScan", true},
        {"Compat/FlacEnableId3v2", false},
        {"Compat/MpegEnableId3v1", false},
        {"Filter/SafeFilenames", false},
        {"Filter/ScaleImages", false},
        {"Filter/ScaleImagesWidth", 200},
        {"Filter/ScaleImagesHeight", 200},
        {"Filter/ParseDiscNumber", false},
        {"UI/Size", QSize(1200, 800)},
    };

    QSettings settings;

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
    QApplication::setApplicationVersion("2.0");

    prepare_settings();

    QApplication app(argc, args);

    QTranslator translator(&app);
    translator.load(":/tr/en.qm");

    app.installTranslator(&translator);

    Coquillo::MainWindow window;
    window.show();

    if (QApplication::arguments().count() > 1) {
        window.addPaths(QApplication::arguments().mid(1));
    }

    return app.exec();
}
