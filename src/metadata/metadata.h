#ifndef COQUILLO_METADATA_METADATA_H
#define COQUILLO_METADATA_METADATA_H

#include <QHash>
#include <QString>
#include <QVariant>

#include "image.h"
#include "tag.h"

namespace Coquillo {
    namespace MetaData {
        typedef QVariantMap Properties;

        class Mapper;

        /**
         * NOTE: Field names are in lowercased letters. Words are separated
         * with underscores.
         */
        class MetaData {
            public:
                static void registerMapper(const QString & tag, Mapper * mapper);
                static Mapper * getMapper(const QString & tag);

                MetaData() { }
                MetaData(const QString & path) : _path(path) { }

                void addTag(const QString & name, const Tag & tag);
                void removeTag(const QString & name);
                void setPrimaryTag(const QString & name);
                bool hasTag(const QString & name) const;
                bool has(const QString & key) const;
                bool has(const QString & key, const QString & tag) const;

                Tag primaryTag() const { return _tags.value(_primary); }
                Tag tag(const QString & name) const { return _tags.value(name); }
                void insert(const QString & key, const QVariant & value);
                QVariant value(const QString & key, const QString & tag) const;
                QVariant value(const QString & key) const { return value(key, _primary); }
                QVariant operator[](const QString & key) const { return value(key, _primary); }
                QStringList fields() const;
                QStringList fields(const QString & tag) const;

                inline QString primaryTagName() const { return _primary; }
                inline QHash<QString, Tag> tags() const { return _tags; }
                inline void setProperties(const Properties & p) { _properties = p; }
                inline Properties properties() const { return _properties; }
                inline void setPath(const QString & path) { _path = path; }
                inline QString path() const { return _path; }

                void setImages(const ImageList & images) { _images = images; }
                const ImageList images() const { return _images; }
                ImageList & images() { return _images; }

            private:
                QString _path;
                QString _primary;
                QHash<QString, Tag> _tags;
                ImageList _images;
                Properties _properties;

                /*
                 * NOTE: Default set of mappers defined in mapper.h
                 */
                static QHash<QString, Mapper*> mappers;
        };
    }
}

Q_DECLARE_METATYPE(Coquillo::MetaData::MetaData);

#endif
