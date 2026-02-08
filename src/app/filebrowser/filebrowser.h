#ifndef FILEBROWSER_H
#define FILEBROWSER_H

#include "filebrowser/filebrowsermodel.h"
#include <QWidget>

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

private:
  Ui::FileBrowser *ui;
  FileBrowserModel *fs;
};
} // namespace Coquillo

#endif
