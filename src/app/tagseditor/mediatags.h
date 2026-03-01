#ifndef COQUILLO_MEDIATAGS_H
#define COQUILLO_MEDIATAGS_H

#include "../selectionnotifier.h"
#include <QDataWidgetMapper>
#include <QPointer>
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

    void setSelectionNotifier(SelectionNotifier *selectionNotifier);

  private:
    void clear();

    Ui::MediaTags *ui;
    QPointer<QDataWidgetMapper> mapper;
    QPointer<SelectionNotifier> selection;
};
} // namespace Coquillo

#endif
