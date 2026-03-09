#include "patternparse.h"
#include "mediastoragemodelcolumns.h"
#include "mediastoragemodelroles.h"
#include "ui_patternparse.h"
#include <qcheckbox.h>
#include <qcontainerfwd.h>
#include <qfileinfo.h>
#include <qnamespace.h>
#include <qregularexpression.h>

const QMap<int, QString> parse_filename(const QString &filePath, const QString &pattern,
                                        const QHash<QString, int> &mappings) {
    const QList<int> numericColumns = {
        Coquillo::MediaStorageModelColumns::column("TRACKNUMBER"),
        Coquillo::MediaStorageModelColumns::column("TRACKTOTAL"),
        Coquillo::MediaStorageModelColumns::column("DISCNUMBER"),
        Coquillo::MediaStorageModelColumns::column("DATE"),
    };

    const auto suffix = QString(".%1").arg(QFileInfo(filePath).suffix());
    const auto basePath = filePath.sliced(0, filePath.length() - suffix.length());

    QMap<int, QString> tags;
    QStringList tokens;
    QString rxPattern(QRegularExpression::escape(pattern) + '$');

    int slength = QRegularExpression::escape("%x").length();

    for (int i = rxPattern.indexOf('%'); i != -1; i = rxPattern.indexOf('%', i + 2)) {
        if (rxPattern.sliced(i + 1, 1) == '%') {
            continue;
        }

        const auto token = rxPattern.sliced(i, 2);

        int column = mappings.value(token, -1);

        if (column != -1 || token == "%i") {
            const QString rxToken =
                numericColumns.contains(column) ? QString("(\\d+)") : QString("([^/\\\\]+)");

            rxPattern.replace(i - slength + 2, slength, rxToken);

            i += rxToken.length() - 2;

            tokens << token;
        }
    }

    auto match = QRegularExpression(rxPattern).match(basePath);

    for (int i = 1; i <= match.lastCapturedIndex(); i++) {
        const auto token = tokens.takeFirst();

        if (token != "%i") {
            int column = mappings[token];

            tags[column] = match.captured(i);
        }
    }

    return tags;
}

namespace Coquillo {
PatternParse::PatternParse(QWidget *parent) : QWidget(parent), ui(new Ui::PatternParse) {
    ui->setupUi(this);

    connect(ui->pattern, &QComboBox::currentTextChanged, this, &PatternParse::updatePreview);
    connect(ui->apply, &QPushButton::clicked, this, &PatternParse::apply);
}

PatternParse::~PatternParse() { delete ui; }

void PatternParse::setSelectionNotifier(SelectionNotifier *selectionNotifier) {
    selection = selectionNotifier;

    connect(selection, &SelectionNotifier::currentChanged, this, &PatternParse::updatePreview);
    connect(selection->model(), &QAbstractItemModel::dataChanged, this,
            &PatternParse::updatePreview);

    updatePreview();
}

const QHash<QString, int> PatternParse::mappings() const {
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

void PatternParse::updatePreview() {
    const auto mappings = this->mappings();
    const auto pattern = ui->pattern->currentText();
    int row = selection->current();

    ui->preview->setText(tr("(Preview)"));

    if (row != -1) {
        const auto idx = selection->model()->index(row, 0);
        const QString path = idx.data(FilePathRole).toString();
        const auto tags = parse_filename(path, pattern, mappings);

        QString preview;

        for (auto it = tags.constKeyValueBegin(); it != tags.constKeyValueEnd(); it++) {
            const auto field = selection->model()->headerData(it->first, Qt::Horizontal).toString();

            preview += QString(", <b>%1:</b> %2").arg(field, it->second);
        }

        if (preview.length()) {
            ui->preview->setText(preview.slice(2));
        } else {
            ui->preview->setText(QString("<i>%1</i>").arg(tr("Pattern does not match.")));
        }
    }
}

void PatternParse::apply() {
    const auto mappings = this->mappings();
    const auto pattern = ui->pattern->currentText();

    for (int row : selection->selection()) {
        const auto idx = selection->model()->index(row, 0);
        const QString path = idx.data(FilePathRole).toString();
        const auto tags = parse_filename(path, pattern, mappings);

        for (auto it = tags.constKeyValueBegin(); it != tags.constKeyValueEnd(); it++) {
            const auto idx = selection->model()->index(row, it->first);

            selection->model()->setData(idx, it->second);
        }
    }
}

} // namespace Coquillo
