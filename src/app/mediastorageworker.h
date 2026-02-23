#ifndef COQUILLO_MEDIASTORAGEWORKER_H
#define COQUILLO_MEDIASTORAGEWORKER_H

#include "src/core/finder.h"
#include "src/core/media.h"
#include <QObject>

namespace Coquillo {
class MediaStorageWorker : public QObject {
    Q_OBJECT

  public:
    MediaStorageWorker(QObject *parent = nullptr);
    ~MediaStorageWorker();

    void findMedia(const QString &path, bool recursive);

    QList<Media> takeBuffer();

  signals:
    void discover(int count);
    void progress(int progress);
    void ready();

  private:
    void readFiles(const coquillo::finder::media_paths &files);
    QList<Media> buffer;
};
} // namespace Coquillo

#endif
