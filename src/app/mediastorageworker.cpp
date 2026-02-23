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

            const auto files = job->future.get();

            readFiles(files);
        }
    });
}

void MediaStorageWorker::readFiles(const coquillo::finder::media_paths &files) {
    auto job = coquillo::finder::read_files(files);

    auto timer = new QTimer();
    timer->start(50);

    emit discover(files.size());

    connect(timer, &QTimer::timeout, [this, timer, job]() {
        const auto batch = job->flush();

        for (const auto item : batch) {
            buffer << std::move(item);
        }

        qDebug() << "PROGRESS" << job->progress;
        emit progress(job->progress);

        if (job->completed) {
            qDebug() << "reading files completed" << job->media.size();

            timer->stop();
            timer->deleteLater();

            emit ready();
        }
    });
}

QList<Media> MediaStorageWorker::takeBuffer() {
    QList<Media> empty;
    std::swap(buffer, empty);

    return empty;
}

} // namespace Coquillo