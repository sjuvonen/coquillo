#ifndef COQUILLO_PATTERNPARSE_H
#define COQUILLO_PATTERNPARSE_H

#include "../selectionnotifier.h"
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class PatternParse;
}
QT_END_NAMESPACE

namespace Coquillo {

class PatternParse : public QWidget {
    Q_OBJECT

  public:
    PatternParse(QWidget *parent = nullptr);
    ~PatternParse();

    void setSelectionNotifier(SelectionNotifier *selectionNotifier);

  private:
    const QHash<QString, int> mappings() const;
    void updatePreview();
    void apply();

    Ui::PatternParse *ui;
    QPointer<SelectionNotifier> selection;
};
} // namespace Coquillo

#endif
