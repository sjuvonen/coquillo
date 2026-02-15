#include "patternparse.h"
#include "ui_patternparse.h"
#include <qcheckbox.h>

namespace Coquillo {
PatternParse::PatternParse(QWidget *parent) : QWidget(parent), ui(new Ui::PatternParse) {
    ui->setupUi(this);
}

PatternParse::~PatternParse() { delete ui; }

} // namespace Coquillo
