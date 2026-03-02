#ifndef COQUILLO_CORE_MEDIA_H
#define COQUILLO_CORE_MEDIA_H

#include "type.h"
#include <QString>
#include <QVariant>
#include <memory>
#include <taglib/fileref.h>

namespace Coquillo {

typedef QMultiMap<QString, QString> Tag;

class Media {
  public:
    static Media fromFileRef(const TagLib::FileRef &ref);

    const QString path() const;
    const QMap<QString, Tag> tags() const;
    const QString get(const QString &field) const;
    void set(const QString &field, const QString &value);

    void addTag(const QString &name, const Tag &values);
    void normalize();
    void reset();

  private:
    Media(Type type, const QString &path);

    class MediaPrivate;
    std::shared_ptr<MediaPrivate> d;
};
} // namespace Coquillo

#endif