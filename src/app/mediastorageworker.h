#ifndef COQUILLO_MEDIASTORAGEWORKER_H
#define COQUILLO_MEDIASTORAGEWORKER_H

#include "src/core/finder.h"
#include <QObject>

namespace Coquillo {
class MediaStorageWorker : public QObject {
    Q_OBJECT

  public:
    MediaStorageWorker(QObject *parent = nullptr);
    ~MediaStorageWorker();

    void findMedia(const QString &path, bool recursive);

  signals:
    void ready(const QStringList &files);

  private:
    void readFiles(const coquillo::finder::media_paths &files);
};
} // namespace Coquillo

#endif
