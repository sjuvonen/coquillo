#ifndef COQUILLO_PATTERNPARSE_H
#define COQUILLO_PATTERNPARSE_H

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

  private:
    Ui::PatternParse *ui;
};
} // namespace Coquillo

#endif
