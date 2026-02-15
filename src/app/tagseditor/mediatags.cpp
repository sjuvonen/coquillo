#include "mediatags.h"
#include "ui_mediatags.h"

namespace Coquillo {
MediaTags::MediaTags(QWidget *parent) : QWidget(parent), ui(new Ui::MediaTags) {
    ui->setupUi(this);
}

MediaTags::~MediaTags() { delete ui; }
} // namespace Coquillo
