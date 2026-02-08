#include "patternrename.h"
#include "ui_patternrename.h"
#include <qcheckbox.h>

namespace Coquillo {
PatternRename::PatternRename(QWidget *parent)
    : QWidget(parent), ui(new Ui::PatternRename) {
  ui->setupUi(this);
}

PatternRename::~PatternRename() { delete ui; }

} // namespace Coquillo
