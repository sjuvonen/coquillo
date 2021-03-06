#ifndef COQUILLO_TAGS_TAGCONTAINER_H
#define COQUILLO_TAGS_TAGCONTAINER_H

#include <QVariantMap>

namespace Coquillo {
    namespace Tags {
        class Tag {
            public:
                typedef QMap<QString, QString> Mapping;

                Tag() { }
                Tag(const QString & type, const Mapping & mapping, const QVariantMap & values);

                QStringList keys() const;
                QVariant value(const QString & field) const;
                QVariantList all(const QString & raw) const;

                bool insert(const QString & field, const QVariant & value);

                inline QVariantMap data() const { return _raw; }
                inline bool isNull() const { return _id.isNull(); }
                inline QString id() const { return _id; }

                bool equals(const QString & field, const QVariant & value) const;

            private:
                QString _id;
                Mapping _mapping;
                QVariantMap _raw;
        };
    }
}

#endif
