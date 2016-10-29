#ifndef COQUILLO_TAGS_TAGCONTAINER_H
#define COQUILLO_TAGS_TAGCONTAINER_H

#include <QVariantHash>

namespace Coquillo {
    namespace Tags {
        class Tag {
            public:
                typedef QHash<QString, QString> Mapping;

                Tag() { }
                Tag(const QString & type, const Mapping & mapping, const QVariantHash & values);

                QStringList keys() const;
                QVariant value(const QString & field) const;
                QVariantList all(const QString & raw) const;

                bool insert(const QString & field, const QVariant & value);

                inline bool isNull() const { return _id.isNull(); }
                inline QString id() const { return _id; }

                bool equals(const QString & field, const QVariant & value) const;

            private:
                QString _id;
                Mapping _mapping;
                QVariantHash _raw;
        };
    }
}

#endif
