#ifndef COQUILLO_PATTERNRENAME_H
#define COQUILLO_PATTERNRENAME_H

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

  private:
    Ui::PatternRename *ui;
};
} // namespace Coquillo

#endif
