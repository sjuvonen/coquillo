#ifndef COQUILLO_MEDIASTORAGE_H
#define COQUILLO_MEDIASTORAGE_H

#include "mediastorageworker.h"
#include "src/core/media.h"
#include <QObject>
#include <qhash.h>
#include <qlist.h>

namespace Coquillo {
class MediaStorage : public QObject {
    Q_OBJECT

  public:
    MediaStorage(QObject *parent = nullptr);
    ~MediaStorage();

    int size() const;
    const Media &at(int index) const;
    Media &at(int index);

  public slots:
    void addPath(const QString &path);
    void removePath(const QString &path);
    void setRecursive(bool recursive);

  signals:
    void started();
    void progress(int progress, int total);
    void finished();

  private:
    bool recursive;
    bool aborted;
    QStringList paths;
    QHash<QString, MediaStorageWorker *> workers;
    QList<Media> media;

    int currentTotal;
    int currentProgress;
};
} // namespace Coquillo

#endif
