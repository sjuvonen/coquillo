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

                QVariant value(const QString & field) const;

                inline bool isNull() const { return _id.isNull(); }
                inline QString id() const { return _id; }

            private:
                QString _id;
                Mapping _mapping;
                QVariantHash _raw;
        };
    }
}

#endif
