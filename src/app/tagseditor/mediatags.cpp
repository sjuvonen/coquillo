#include "mediatags.h"
#include "mediastoragemodelcolumns.h"
#include "modificationindicatordelegate.h"
#include "selectionnotifier.h"
#include "ui_mediatags.h"
#include <QLineEdit>
#include <QSpinBox>
#include <qdatawidgetmapper.h>
#include <qlineedit.h>

namespace Coquillo {
MediaTags::MediaTags(QWidget *parent)
    : QWidget(parent), ui(new Ui::MediaTags), mapper(new QDataWidgetMapper(this)),
      labelMapper(new QDataWidgetMapper(this)) {
    ui->setupUi(this);

    /**
     * NOTE: Qt Designer does not allow setting spinboxes empty (only 0) so we clear manually.
     */
    clear();
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
    mapper->addMapping(ui->tagEncoder, MediaStorageModelColumns::column("ENCODED-BY"));
    mapper->addMapping(ui->tagGenre, MediaStorageModelColumns::column("GENRE"));
    mapper->addMapping(ui->tagOriginalArtist, MediaStorageModelColumns::column("PERFORMER"));
    mapper->addMapping(ui->tagTitle, MediaStorageModelColumns::column("TITLE"));
    mapper->addMapping(ui->tagTrack, MediaStorageModelColumns::column("TRACKNUMBER"));
    mapper->addMapping(ui->tagTrackTotal, MediaStorageModelColumns::column("TRACKTOTAL"));
    mapper->addMapping(ui->tagUrl, MediaStorageModelColumns::column("CONTACT"));
    mapper->addMapping(ui->tagYear, MediaStorageModelColumns::column("DATE"));

    /**
     * The model here is a proxy model: assuming it will not re-order columns.
     */
    labelMapper->setModel(selection->model());
    labelMapper->addMapping(ui->labelAlbum, MediaStorageModelColumns::column("ALBUM"));
    labelMapper->addMapping(ui->labelAlbumArtist, MediaStorageModelColumns::column("ALBUMARTIST"));
    labelMapper->addMapping(ui->labelArtist, MediaStorageModelColumns::column("ARTIST"));
    labelMapper->addMapping(ui->labelComment, MediaStorageModelColumns::column("COMMENT"));
    labelMapper->addMapping(ui->labelComposer, MediaStorageModelColumns::column("COMPOSER"));
    labelMapper->addMapping(ui->labelDisc, MediaStorageModelColumns::column("DISCNUMBER"));
    labelMapper->addMapping(ui->labelEncoder, MediaStorageModelColumns::column("ENCODED-BY"));
    labelMapper->addMapping(ui->labelGenre, MediaStorageModelColumns::column("GENRE"));
    labelMapper->addMapping(ui->labelOriginalArtist, MediaStorageModelColumns::column("PERFORMER"));
    labelMapper->addMapping(ui->labelTitle, MediaStorageModelColumns::column("TITLE"));
    labelMapper->addMapping(ui->labelTrack, MediaStorageModelColumns::column("TRACKNUMBER"));
    labelMapper->addMapping(ui->labelTrackTotal, MediaStorageModelColumns::column("TRACKTOTAL"));
    labelMapper->addMapping(ui->labelUrl, MediaStorageModelColumns::column("CONTACT"));
    labelMapper->addMapping(ui->labelYear, MediaStorageModelColumns::column("DATE"));

    labelMapper->setCurrentIndex(selectionNotifier->current());
    labelMapper->setItemDelegate(new ModificationIndicatorDelegate);

    connect(selection, &SelectionNotifier::currentChanged, [=](int row) {
        /**
         * NOTE: Must clear every time because the data widget mapper will not clear inputs when a
         * field is null.
         */
        clear();

        mapper->setCurrentIndex(row);
        labelMapper->setCurrentIndex(row);
    });
}

void MediaTags::submit() { mapper->submit(); }

void MediaTags::clear() {
    const auto inputs = findChildren<QLineEdit *>();
    const auto spinners = findChildren<QSpinBox *>();

    mapper->setCurrentIndex(-1);

    for (auto input : inputs) {
        input->clear();
    }

    for (auto spinner : spinners) {
        spinner->clear();
    }
}
} // namespace Coquillo
