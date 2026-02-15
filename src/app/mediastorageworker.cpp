#include "mediastorageworker.h"
#include "src/core/finder.h"
#include <QDebug>
#include <QObject>
#include <QTimer>

namespace Coquillo {
MediaStorageWorker::MediaStorageWorker(QObject *parent) : QObject(parent) {}

MediaStorageWorker::~MediaStorageWorker() {}

void MediaStorageWorker::findMedia(const QString &path, bool recursive) {
    qDebug() << "read path into storage" << path;

    auto job = coquillo::finder::find_files(path.toStdString(), recursive);

    auto timer = new QTimer(this);
    timer->start(100);

    connect(timer, &QTimer::timeout, [this, timer, job]() {
        if (job->completed) {
            timer->stop();
            timer->deleteLater();

            auto files = job->future.get();
            qDebug() << "reading directory completed" << files.size();

            readFiles(files);
        }
    });
}

void MediaStorageWorker::readFiles(const coquillo::finder::media_paths &files) {
    qDebug() << "read tags from files" << files.size();

    auto job = coquillo::finder::read_files(files);

    auto timer = new QTimer();
    timer->start(100);

    connect(timer, &QTimer::timeout, [this, timer, job]() {
        if (job->completed) {
            qDebug() << "reading files completed" << job->media.size();

            timer->stop();
            timer->deleteLater();

            emit ready({});
        }
    });
}

} // namespace Coquillo