#ifndef COQUILLO_MEDIAIMAGES_H
#define COQUILLO_MEDIAIMAGES_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MediaImages;
}
QT_END_NAMESPACE

namespace Coquillo {

class MediaImages : public QWidget {
    Q_OBJECT

  public:
    MediaImages(QWidget *parent = nullptr);
    ~MediaImages();

  private:
    Ui::MediaImages *ui;
};
} // namespace Coquillo

#endif
