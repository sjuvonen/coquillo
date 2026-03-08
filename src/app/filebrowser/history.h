#ifndef COQUILLO_FILEBROWSER_HISTORY_H
#define COQUILLO_FILEBROWSER_HISTORY_H

#include <qobject.h>

namespace Coquillo {
class History : public QObject {
    Q_OBJECT

  public:
    History(QObject *parent = nullptr);

    void add(const QString &path);
    void remove(int index);

    const QStringList entries() const;

  private:
    void load();
    void save();

    QStringList history;
};
} // namespace Coquillo

#endif