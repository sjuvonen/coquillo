#include "mediatags.h"
#include "mediastoragemodelcolumns.h"
#include "selectionnotifier.h"
#include "ui_mediatags.h"

namespace Coquillo {
MediaTags::MediaTags(QWidget *parent)
    : QWidget(parent), ui(new Ui::MediaTags), mapper(new QDataWidgetMapper(this)) {
    ui->setupUi(this);
}

MediaTags::~MediaTags() { delete ui; }

void MediaTags::setSelectionNotifier(SelectionNotifier *selectionNotifier) {
    selection = selectionNotifier;

    /**
     * The model here is a proxy model: assuming it will not re-order columns.
     */
    mapper->setModel(selection->model());
    mapper->addMapping(ui->tagAlbum, MediaStorageModelColumns::column("ALBUM"));
    mapper->addMapping(ui->tagAlbumArtist, MediaStorageModelColumns::column("ALBUMARTIST"));
    mapper->addMapping(ui->tagArtist, MediaStorageModelColumns::column("ARTIST"));
    mapper->addMapping(ui->tagComment, MediaStorageModelColumns::column("COMMENT"));
    mapper->addMapping(ui->tagComposer, MediaStorageModelColumns::column("COMPOSER"));
    mapper->addMapping(ui->tagDisc, MediaStorageModelColumns::column("DISCNUMBER"));
    mapper->addMapping(ui->tagEncoder, MediaStorageModelColumns::column("ENCODER"));
    mapper->addMapping(ui->tagGenre, MediaStorageModelColumns::column("GENRE"));
    mapper->addMapping(ui->tagOriginalArtist, MediaStorageModelColumns::column("PERFORMER"));
    mapper->addMapping(ui->tagTitle, MediaStorageModelColumns::column("TITLE"));
    mapper->addMapping(ui->tagTrack, MediaStorageModelColumns::column("TRACKNUMBER"));
    mapper->addMapping(ui->tagTrackTotal, MediaStorageModelColumns::column("TRACKTOTAL"));
    mapper->addMapping(ui->tagUrl, MediaStorageModelColumns::column("URL"));
    mapper->addMapping(ui->tagYear, MediaStorageModelColumns::column("YEAR"));

    mapper->setCurrentIndex(selectionNotifier->current());

    connect(selection, &SelectionNotifier::currentChanged, [=](int row) {
        mapper->setCurrentIndex(row);

        if (row == -1) {
            // mapper->revert();
        }
    });
}
} // namespace Coquillo
