#include "mediametadata.h"
#include "metadatamodel.h"
#include "selectionnotifier.h"
#include "ui_mediametadata.h"

namespace Coquillo {
MediaMetadata::MediaMetadata(QWidget *parent)
    : QWidget(parent), ui(new Ui::MediaMetadata), model(new MetadataModel) {
    ui->setupUi(this);

    ui->metadataView->setModel(model);

    connect(ui->tag, &QComboBox::currentTextChanged, model, &MetadataModel::setTag);
}

MediaMetadata::~MediaMetadata() { delete ui; }

void MediaMetadata::setMediaStorage(MediaStorage *mediaStorage) {
    storage = mediaStorage;
    model->setStorage(mediaStorage);
}

void MediaMetadata::setSelectionNotifier(SelectionNotifier *selectionNotifier) {
    selection = selectionNotifier;

    connect(selection, &SelectionNotifier::currentChanged, [=](int row) {
        ui->tag->clear();

        if (row != -1) {
            const auto media = storage->at(row);
            const auto tags = media.tags().keys();

            ui->tag->addItems(tags);
            ui->tag->setCurrentText(media.primary());

            model->setTag(media.primary());
            model->setRow(row);
        }
    });
}

} // namespace Coquillo
