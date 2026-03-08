#include "patternrename.h"
#include "mediastoragemodelcolumns.h"
#include "mediastoragemodelroles.h"
#include "selectionnotifier.h"
#include "ui_patternrename.h"
#include <qabstractitemmodel.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qdatawidgetmapper.h>
#include <qdir.h>
#include <qfileinfo.h>
#include <qpushbutton.h>
#include <qregularexpression.h>

const QString apply_rename_pattern(const QModelIndex &row, const QString &pattern,
                                   const QHash<QString, int> &mappings) {
    QString path = row.data(Coquillo::FilePathRole).toString();
    QString renamed(pattern);

    /**
     * NOTE: Always increment by +2 in `for` to avoid infinite looping in case of no replacement.
     */
    for (int i = renamed.indexOf('%'); i != -1; i = renamed.indexOf('%', i + 2)) {
        const auto token = renamed.sliced(i, 2);
        int column = mappings.value(token, -1);

        if (column != -1) {
            const auto value = row.siblingAtColumn(column).data().toString();

            renamed.replace(i, 2, value);

            i += value.length() - 2;
        }
    }

    const QString dir = QFileInfo(path).absoluteDir().path();
    const QString suffix = QFileInfo(path).suffix();

    const QString canonical =
        QFileInfo(QString("%1/%2.%3").arg(dir, renamed, suffix)).absoluteFilePath();

    return canonical;
}

namespace Coquillo {
PatternRename::PatternRename(QWidget *parent) : QWidget(parent), ui(new Ui::PatternRename) {
    ui->setupUi(this);

    connect(ui->pattern, &QComboBox::currentTextChanged, this, &PatternRename::updatePreview);
    connect(ui->apply, &QPushButton::clicked, this, &PatternRename::apply);
}

PatternRename::~PatternRename() { delete ui; }

void PatternRename::setSelectionNotifier(SelectionNotifier *selectionNotifier) {
    selection = selectionNotifier;

    connect(selection, &SelectionNotifier::currentChanged, this, &PatternRename::updatePreview);
    connect(selection->model(), &QAbstractItemModel::dataChanged, this,
            &PatternRename::updatePreview);

    updatePreview();
}

const QHash<QString, int> PatternRename::mappings() const {
    const QHash<QString, int> mappings({
        {ui->labelArtist->text().slice(0, 2), MediaStorageModelColumns::column("ARTIST")},
        {ui->labelAlbum->text().slice(0, 2), MediaStorageModelColumns::column("ALBUM")},

        {ui->labelAlbumArtist->text().slice(0, 2), MediaStorageModelColumns::column("ALBUMARTIST")},

        {ui->labelComment->text().slice(0, 2), MediaStorageModelColumns::column("COMMENT")},
        {ui->labelComposer->text().slice(0, 2), MediaStorageModelColumns::column("COMPOSER")},
        {ui->labelDiscNumber->text().slice(0, 2), MediaStorageModelColumns::column("DISCNUMBER")},
        {ui->labelGenre->text().slice(0, 2), MediaStorageModelColumns::column("GENRE")},
        {ui->labelOriginalArtist->text().slice(0, 2),
         MediaStorageModelColumns::column("PERFORMER")},
        {ui->labelTitle->text().slice(0, 2), MediaStorageModelColumns::column("TITLE")},
        {ui->labelTrackNumber->text().slice(0, 2), MediaStorageModelColumns::column("TRACKNUMBER")},
        {ui->labelTrackTotal->text().slice(0, 2), MediaStorageModelColumns::column("TRACKTOTAL")},
        {ui->labelYear->text().slice(0, 2), MediaStorageModelColumns::column("DATE")},
    });

    return mappings;
}

void PatternRename::updatePreview() {
    const auto mappings = this->mappings();
    const auto pattern = ui->pattern->currentText();
    int row = selection->current();

    if (row == -1) {
        ui->preview->setText(tr("(Preview)"));
    } else {
        const auto idx = selection->model()->index(row, 0);
        const auto renamed = apply_rename_pattern(idx, pattern, mappings);

        ui->preview->setText(renamed);
    }
}

void PatternRename::apply() {
    const auto mappings = this->mappings();
    const auto pattern = ui->pattern->currentText();

    for (int row : selection->selection()) {
        const auto idx = selection->model()->index(row, MediaStorageModelColumns::pathColumn());
        const auto renamed = apply_rename_pattern(idx, pattern, mappings);

        qDebug() << "rename" << idx.row() << renamed;

        selection->model()->setData(idx, renamed);
    }
}

} // namespace Coquillo
