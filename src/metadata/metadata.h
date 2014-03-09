#ifndef COQUILLO_METADATA_METADATA_H
#define COQUILLO_METADATA_METADATA_H

#include <QHash>
#include <QString>
#include <QVariant>

namespace Coquillo {
    namespace MetaData {
        typedef QHash<QString, QVariant> Tag;
        class MetaData {
            public:
                MetaData() { }
                MetaData(const QString & path) : _path(path) { }

                void addTag(const QString & name, const Tag & tag);
                void removeTag(const QString & name);
                void setPrimaryTag(const QString & name);
                bool hasTag(const QString & name);
                bool has(const QString & key);
                bool has(const QString & key, const QString & tag);

                Tag tag(const QString & name) const { return _tags.value(name); }
                void insert(const QString & key, const QVariant & value);
                QVariant value(const QString & key, const QString & tag) const;
                QVariant value(const QString & key) const { return value(key, _primary); }
                QVariant operator[](const QString & key) const { return value(key, _primary); }
                inline void setPath(const QString & path) { _path = path; }
                inline QString path() const { return _path; }

            private:
                QString _path;
                QString _primary;
                QHash<QString, Tag> _tags;
        };
    }
}

Q_DECLARE_METATYPE(Coquillo::MetaData::MetaData);

#endif
