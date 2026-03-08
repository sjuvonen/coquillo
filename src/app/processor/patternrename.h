#ifndef COQUILLO_PATTERNRENAME_H
#define COQUILLO_PATTERNRENAME_H

#include "../selectionnotifier.h"
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class PatternRename;
}
QT_END_NAMESPACE

namespace Coquillo {

class PatternRename : public QWidget {
    Q_OBJECT

  public:
    PatternRename(QWidget *parent = nullptr);
    ~PatternRename();

    void setSelectionNotifier(SelectionNotifier *selectionNotifier);

  private:
    const QHash<QString, int> mappings() const;
    void updatePreview();
    void apply();

    Ui::PatternRename *ui;
    QPointer<SelectionNotifier> selection;
};
} // namespace Coquillo

#endif
