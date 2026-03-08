#ifndef COQUILLO_FILEBROWSER_FILEBROWSER_H
#define COQUILLO_FILEBROWSER_FILEBROWSER_H

#include "bookmarks.h"
#include "filebrowsermodel.h"
#include "history.h"
#include <QWidget>
#include <qpointer.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class FileBrowser;
}
QT_END_NAMESPACE

namespace Coquillo {

class FileBrowser : public QWidget {
    Q_OBJECT

  public:
    FileBrowser(QWidget *parent = nullptr);
    ~FileBrowser();

    void setDirectory(const QString &directory);
    void setRecursive(bool recursive);

  signals:
    void pathAdded(const QString &path, bool recursive);
    void pathRemoved(const QString &path, bool recursive);
    void recursiveToggled(bool recursive);

  private:
    void setupBookmarks();

    Ui::FileBrowser *ui;
    QPointer<FileBrowserModel> fs;
    QPointer<Bookmarks> bookmarks;
    QPointer<History> history;
};
} // namespace Coquillo

#endif
