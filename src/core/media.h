#ifndef COQUILLO_CORE_MEDIA_H
#define COQUILLO_CORE_MEDIA_H

#include "type.h"
#include <QString>
#include <QVariant>
#include <memory>
#include <taglib/fileref.h>

namespace Coquillo {

typedef QHash<QString, QString> QStringHash;

class Media {
  public:
    static Media fromFileRef(const TagLib::FileRef &ref);

    const QString path() const;
    const QString get(const QString &field) const;
    void set(const QString &field, const QString &value);

  private:
    Media(Type type, const QString &path, const QStringHash &data);

    class MediaPrivate;
    std::shared_ptr<MediaPrivate> d;
};
} // namespace Coquillo

#endif