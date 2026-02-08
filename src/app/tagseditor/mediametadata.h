#ifndef MEDIAMETADATA_H
#define MEDIAMETADATA_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MediaMetadata;
}
QT_END_NAMESPACE

namespace Coquillo {

class MediaMetadata : public QWidget {
  Q_OBJECT

public:
  MediaMetadata(QWidget *parent = nullptr);
  ~MediaMetadata();

private:
  Ui::MediaMetadata *ui;
};
} // namespace Coquillo

#endif
