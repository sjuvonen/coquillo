#include "mediaimages.h"
#include "ui_mediaimages.h"

namespace Coquillo {
MediaImages::MediaImages(QWidget *parent)
    : QWidget(parent), ui(new Ui::MediaImages) {
  ui->setupUi(this);
}

MediaImages::~MediaImages() { delete ui; }
} // namespace Coquillo
