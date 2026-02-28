#include "mediastoragemodelcolumns.h"

#include <QObject>

namespace Coquillo {
QStringList MediaStorageModelColumns::labels({
    QString(),
    QObject::tr("Filename"),
    QObject::tr("Title"),
    QObject::tr("Artist"),
    QObject::tr("Album"),
    QObject::tr("Genre"),
    QObject::tr("Comment"),
    QObject::tr("Year"),
    QObject::tr("#"),
    QObject::tr("Total"),
    QObject::tr("Disc"),
    QObject::tr("Original Artist"),
    QObject::tr("Album Artist"),
    QObject::tr("Composer"),
    QObject::tr("URL"),
    QObject::tr("Encoder"),
    QObject::tr("Images"),
});

QStringList MediaStorageModelColumns::fields({
    QString(),
    QString(),
    "TITLE",
    "ARTIST",
    "ALBUM",
    "GENRE",
    "COMMENT",
    "YEAR",
    "TRACKNUMBER",
    "TRACKTOTAL",
    "DISCNUMBER",
    "PERFORMER",
    "ALBUMARTIST",
    "COMPOSER",
    "URL",
    "ENCODER",
    QString(),
});

const QString MediaStorageModelColumns::label(int column) { return labels.value(column); }

const QString MediaStorageModelColumns::labelFor(const QString &field) {
    return labels.value(fields.indexOf(field));
}

const QString MediaStorageModelColumns::field(int column) { return fields.value(column); }

const QString MediaStorageModelColumns::fieldFor(const QString &label) {
    return fields.value(labels.indexOf(label));
}

int MediaStorageModelColumns::column(const QString &field) { return fields.indexOf(field); }

int MediaStorageModelColumns::size() { return fields.size(); }

} // namespace Coquillo