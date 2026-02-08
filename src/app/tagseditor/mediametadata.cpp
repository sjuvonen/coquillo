#include "mediametadata.h"
#include "ui_mediametadata.h"

namespace Coquillo {
MediaMetadata::MediaMetadata(QWidget *parent)
    : QWidget(parent), ui(new Ui::MediaMetadata) {
  ui->setupUi(this);
}

MediaMetadata::~MediaMetadata() { delete ui; }
} // namespace Coquillo
