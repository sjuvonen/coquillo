#include <QApplication>
#include "mainwindow.h"

#include <QVariantMap>
#include "searcher/musicbrainz.h"

int main(int argc, char ** args) {
    QApplication::setOrganizationName("Juvonet");
    QApplication::setOrganizationDomain("juvonet.fi");
    QApplication::setApplicationName("Coquillo");
    QApplication::setApplicationVersion("0.1");

    QApplication app(argc, args);
    Coquillo::MainWindow * window = new Coquillo::MainWindow;
    window->show();

    QVariantMap search;
    search["artist"] = "scooter";
    search["title"] = "push the beat";
    Coquillo::Searcher::MusicBrainz searcher;
    searcher.search(search);

    return app.exec();
}
