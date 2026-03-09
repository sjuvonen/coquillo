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

    const QString renamed() const;
    void rename(const QString &newPath);

    const QString primary() const;
    const QMap<QString, Tag> tags() const;
    void addTag(const QString &name, const Tag &values);

    const QString get(const QString &field) const;
    void set(const QString &field, const QString &value);

    int imageCount() const;
    void setImageCount(int count);

    void normalize();
    void reset();
    void reset(const QString &field);

    bool changed() const;
    bool changed(const QString &field) const;

  private:
    Media(Type type, const QString &path);

    class MediaPrivate;
    std::shared_ptr<MediaPrivate> d;
};
} // namespace Coquillo

#endif