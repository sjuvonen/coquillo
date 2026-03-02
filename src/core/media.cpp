#include "media.h"
#include "files.h"
#include "type.h"
#include <QVariantHash>
#include <memory>
#include <qlogging.h>
#include <qregularexpression.h>
#include <taglib/tpropertymap.h>
#include <taglib/tstring.h>

namespace Coquillo {
class Media::MediaPrivate {
  public:
    MediaPrivate(Type type, const QString &path);
    void addTag(const QString &name, const Tag &values);
    void normalize();
    void reset();

    Type type;
    QString path;
    QString rename;

    /**
     * Primary tag name.
     */
    QString primary;

    /**
     * Raw data.
     */
    QMap<QString, Tag> tags;

    /**
     * Normalized tags.
     */
    Tag values;

    /**
     * Modifications to tags.s
     */
    Tag updates;
};

Media::MediaPrivate::MediaPrivate(Type type, const QString &path) : type(type), path(path) {}

void Media::MediaPrivate::addTag(const QString &name, const Tag &values) {
    if (primary.isNull()) {
        primary = name;
    }

    tags[name] = values;
}

void Media::MediaPrivate::normalize() {
    if (primary == "VorbisComment" || primary == "ID3v1" || primary == "Common") {
        values = tags[primary];
    } else if (primary == "ID3v2") {
        const auto tag = tags[primary];

        values = {
            {"COMMENT", tag.value("COMM")},     {"ALBUM", tag.value("TALB")},
            {"COMPOSER", tag.value("TCOM")},    {"GENRE", tag.value("TCON")},
            {"COPYRIGHT", tag.value("TCOP")},   {"DATE", tag.value("TDRC")},
            {"ENCODED-BY", tag.value("TENC")},  {"TITLE", tag.value("TIT2")},
            {"PERFORMER", tag.value("TOPE")},   {"ARTIST", tag.value("TPE1")},
            {"ALBUMARTIST", tag.value("TPE2")}, {"DISCNUMBER", tag.value("TPOS")},
            {"TRACKNUMBER", tag.value("TRCK")}, {"CONTACT", tag.value("WXXX")},
        };

        /**
         * URL ("WXXX") is a list of multiple frames indicated by prefix "[] " or "[FOO] ".
         */
        // if (!values.value("CONTACT").isNull()) {
        //     // qDebug() << "term:" << values.value("CONTACT").indexOf('\0');

        //     qDebug() << "\n\n\nHELLLLOOOO\n\n\n" << values.value("CONTACT").indexOf('\0');

        //     // const auto url = values.value("CONTACT").remove(QRegularExpression("^\\[\\] "));

        //     // values.replace("CONTACT", url);
        // }

        if (!values.value("TRACKNUMBER").isNull()) {
            const auto trackAndTotal = values.value("TRACKNUMBER").split("/");

            // ID3 tags only use a single field in format "01/12".
            if (trackAndTotal.size() > 1) {
                values.replace("TRACKNUMBER", trackAndTotal[0]);
                values.replace("TRACKTOTAL", trackAndTotal[1]);
            }
        }

        if (!values.value("DISCNUMBER").isNull()) {
            const auto discAndTotal = values.value("DISCNUMBER").split("/");

            // ID3 tags only use a single field in format "01/12".
            if (discAndTotal.size() > 1) {
                values.replace("DISCNUMBER", discAndTotal[0]);
                values.replace("DISCTOTAL", discAndTotal[1]);
            }
        }
    }
}

void Media::MediaPrivate::reset() {
    rename.clear();
    updates.clear();
}

Media::Media(Type type, const QString &path)
    : d(std::make_shared<Media::MediaPrivate>(type, path)) {}

Media Media::fromFileRef(const TagLib::FileRef &ref) {
    /**
     * FIXME: Presumably will fail on Windows since local encoding is not UTF-8?
     */
    const QString path = QString::fromUtf8(ref.file()->name());
    Type type = coquillo::files::file_type(ref);
    Media media(type, path);

    coquillo::files::try_read_mpeg(*ref.file(), media);
    coquillo::files::try_read_flac(*ref.file(), media);
    coquillo::files::try_read_vorbis(*ref.file(), media);

    media.normalize();

    qDebug() << path;
    qDebug() << media.tags();
    qDebug() << "";

    return std::move(media);
}

const QString Media::path() const { return d->path; }

const QMap<QString, Tag> Media::tags() const { return d->tags; }

const QString Media::get(const QString &field) const {
    if (d->updates.contains(field)) {
        return d->updates.value(field);
    } else {
        return d->values.value(field);
    }
}

void Media::set(const QString &field, const QString &value) { d->updates.replace(field, value); }

void Media::addTag(const QString &name, const Tag &values) { d->addTag(name, values); }

void Media::normalize() { d->normalize(); }

void Media::reset() { d->reset(); }
} // namespace Coquillo