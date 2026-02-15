#ifndef COQUILLO_MEDIATAGS_H
#define COQUILLO_MEDIATAGS_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MediaTags;
}
QT_END_NAMESPACE

namespace Coquillo {

class MediaTags : public QWidget {
    Q_OBJECT

  public:
    MediaTags(QWidget *parent = nullptr);
    ~MediaTags();

  private:
    Ui::MediaTags *ui;
};
} // namespace Coquillo

#endif
