#ifndef COQUILLO_FILEBROWSER_BOOKMARKS_H
#define COQUILLO_FILEBROWSER_BOOKMARKS_H

#include <QObject>

namespace Coquillo {
struct Bookmark {
    Bookmark(const QString &path, const QString &label);

    QString path;
    QString label;
};

class Bookmarks : public QObject {
    Q_OBJECT

  public:
    Bookmarks(QObject *parent = nullptr);

    inline const QList<Bookmark> entries() const;

    void add(const QString &path, const QString &label);
    void remove(int index);

  private:
    void loadLegacy();
    void load();
    void save();

    QList<Bookmark> _entries;
};

const QList<Bookmark> Bookmarks::entries() const { return _entries; }
} // namespace Coquillo

#endif