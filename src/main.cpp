#include <QApplication>
#include "mainwindow.h"

#include <QDebug>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QVariantHash>

#include <processor/patterns.h>
#include <webtags/tagsearchdialog.h>

#include <metadata/metadatamodel.h>
#include <quick/metadataproxymodel.h>

int main(int argc, char ** args) {
    QApplication::setOrganizationName("Juvonet");
    QApplication::setOrganizationDomain("juvonet.fi");
    QApplication::setApplicationName("Coquillo");
    QApplication::setApplicationVersion("0.1");

    QApplication app(argc, args);

    if (QApplication::arguments().contains("--quick")) {
        Coquillo::MetaDataModel * model = new Coquillo::MetaDataModel;
        model->addDirectory("/home/samu/Music/Swipe Me");
        Coquillo::Quick::MetaDataProxyModel * proxy = new Coquillo::Quick::MetaDataProxyModel;
        proxy->setSourceModel(model);

        QQmlApplicationEngine * engine = new QQmlApplicationEngine();
        engine->rootContext()->setContextProperty("metaData", proxy);
        engine->load("quick/coquillo.qml");
    } else {
        Coquillo::MainWindow * window = new Coquillo::MainWindow;
        window->show();
//         Coquillo::WebTags::TagSearchDialog * dialog = new Coquillo::WebTags::TagSearchDialog;
//         dialog->show();
    }

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
