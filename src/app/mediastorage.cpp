#include "mediastorage.h"
#include "mediastorageworker.h"
#include <qdebug.h>
#include <qlogging.h>
#include <qobject.h>

namespace Coquillo {

MediaStorage::MediaStorage(QObject *parent)
    : QObject(parent), recursive(false), aborted(false), currentTotal(0), currentProgress(0) {
    qDebug() << "media storage created" << recursive;
}

MediaStorage::~MediaStorage() {}

int MediaStorage::size() const { return media.size(); }

const Media &MediaStorage::at(int index) const { return media.at(index); }

Media &MediaStorage::at(int index) { return media[index]; }

void MediaStorage::addPath(const QString &path) {
    if (workers.contains(path)) {
        return;
    }

    if (workers.empty()) {
        emit started();
    }

    auto worker = new MediaStorageWorker(this);
    worker->findMedia(path, recursive);

    paths << path;
    workers[path] = worker;

    connect(worker, &MediaStorageWorker::discover, [this](int count) {
        currentTotal += count;

        emit progress(currentProgress, currentTotal);
    });

    connect(worker, &MediaStorageWorker::progress, [this, worker](int slice) {
        media << worker->takeBuffer();
        currentProgress += slice;

        emit progress(currentProgress, currentTotal);
    });

    connect(worker, &MediaStorageWorker::ready, [this, worker, path]() {
        workers.remove(path);
        worker->deleteLater();

        if (workers.empty()) {
            currentTotal = 0;
            currentProgress = 0;
            emit finished();
        }
    });
}

void MediaStorage::removePath(const QString &path) {
    paths.removeAll(path);

    qDebug() << "remove path" << path << "from storage";
}

void MediaStorage::setRecursive(bool recursive) {
    this->recursive = recursive;

    if (recursive) {
        qDebug() << "find media recursively";
    } else {
        qDebug() << "remove recursively found media";
    }
}
} // namespace Coquillo