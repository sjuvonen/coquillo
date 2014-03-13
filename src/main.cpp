#include <QApplication>
#include "mainwindow.h"

#include <QDebug>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QVariantHash>

#include <metadata/metadatamodel.h>
#include <quick/metadataproxymodel.h>

int main(int argc, char ** args) {
    QApplication::setOrganizationName("Juvonet");
    QApplication::setOrganizationDomain("juvonet.fi");
    QApplication::setApplicationName("Coquillo");
    QApplication::setApplicationVersion("0.1");

    QApplication app(argc, args);

    if (QApplication::arguments().contains("--quick")) {
        Coquillo::MetaData::MetaDataModel * model = new Coquillo::MetaData::MetaDataModel;
        model->addDirectory("/home/samu/Music/Swipe Me");
        Coquillo::Quick::MetaDataProxyModel * proxy = new Coquillo::Quick::MetaDataProxyModel;
        proxy->setSourceModel(model);

        QQmlApplicationEngine * engine = new QQmlApplicationEngine();
        engine->rootContext()->setContextProperty("metaData", proxy);
        engine->load("quick/coquillo.qml");
    } else {
        Coquillo::MainWindow * window = new Coquillo::MainWindow;
        window->show();
    }

    return app.exec();
}
