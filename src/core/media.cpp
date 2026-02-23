#include "media.h"
#include "files.h"
#include <QVariantHash>
#include <memory>
#include <taglib/tpropertymap.h>
#include <taglib/tstring.h>

namespace Coquillo {
class Media::MediaPrivate {
  public:
    MediaPrivate(Type type, const QString &path, const QStringHash &data);
    void add(const QString &tag, const QVariantHash &data);
    void markForDelete(const QString &tag);
    void reset();

    Type type;
    QString path;
    QString rename;
    const QStringHash data;
};

Media::MediaPrivate::MediaPrivate(Type type, const QString &path, const QStringHash &data)
    : type(type), path(path), data(data) {}

Media Media::fromFileRef(const TagLib::FileRef &ref) {
    /**
     * FIXME: Presumably will fail on Windows since local encoding is not UTF-8?
     */
    const QString path = QString::fromUtf8(ref.file()->name());
    Type type = coquillo::files::file_type(ref);
    QStringHash data;

    for (const auto it : ref.properties()) {
        if (it.second.size()) {
            data[TStringToQString(it.first)] = TStringToQString(it.second.front());
        }
    }

    return Media(type, path, data);
}

const QString Media::path() const { return d->path; }

const QString Media::get(const QString &field) const { return d->data[field]; }

Media::Media(Type type, const QString &path, const QStringHash &data)
    : d(std::make_shared<Media::MediaPrivate>(type, path, data)) {}
} // namespace Coquillo