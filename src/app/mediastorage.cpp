#include "mediastorage.h"
#include "mediastorageworker.h"
#include <qdebug.h>
#include <qlogging.h>
#include <qobject.h>

namespace Coquillo {

MediaStorage::MediaStorage(QObject *parent) : QObject(parent), recursive(false), aborted(false) {
    qDebug() << "media storage created" << recursive;
}

MediaStorage::~MediaStorage() {}

void MediaStorage::addPath(const QString &path) {
    paths << path;

    auto worker = new MediaStorageWorker(this);
    worker->findMedia(path, recursive);

    emit started();

    connect(worker, &MediaStorageWorker::ready, [this, worker](const QStringList &files) {
        worker->deleteLater();

        emit finished();
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