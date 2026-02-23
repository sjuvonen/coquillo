#include "app/mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QLocale>
#include <QSettings>
#include <QStandardPaths>
#include <QTranslator>

int main(int argc, char *argv[]) {
    QApplication::setOrganizationName("Juvonet");
    QApplication::setOrganizationDomain("juvonet.fi");
    QApplication::setApplicationName("Coquillo");
    QApplication::setApplicationVersion("3.0");

    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();

    qDebug() << "UNICODE" << "ä" << "ö" << "Ä" << "Ö" << "é" << "É";
    qDebug() << "UNICODE" << L"ä" << L"ö" << L"Ä" << L"Ö" << L"é" << L"É";

    for (const QString &locale : uiLanguages) {
        const QString baseName = "Coquillo_" + QLocale(locale).name();

        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    Coquillo::MainWindow mainWindow;
    mainWindow.show();

    return a.exec();
}
