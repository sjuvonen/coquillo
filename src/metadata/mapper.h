#ifndef COQUILLO_METADATA_MAPPER_H
#define COQUILLO_METADATA_MAPPER_H

#include <QHash>
#include <QString>
#include <QVariant>

namespace Coquillo {
    namespace MetaData {
        class Mapper {
            public:
                virtual ~Mapper() { }
                QString mapToSource(const QString & name) const;
                QString mapFromSource(const QString & name) const;

                virtual QVariant value(const QVariantMap & tag, const QString & field) const;
                virtual void insert(QVariantMap & tag, const QString & field, const QVariant & value);

            protected:
                QHash<QString, QString> _map;
        };

        class DummyMapper : public Mapper {
            public:
                QVariant value(const QVariantMap & tag, const QString & field) const ;
                void insert(QVariantMap & tag, const QString & field, const QVariant & value);
        };

        class Id3v2Mapper : public Mapper {
            public:
                Id3v2Mapper();
                QVariant value(const QVariantMap & tag, const QString & field) const ;
                void insert(QVariantMap & tag, const QString & field, const QVariant & value);
        };

        class XiphMapper : public Mapper {
            public:
                XiphMapper();
                QVariant value(const QVariantMap & tag, const QString & field) const;
                void insert(QVariantMap & tag, const QString & field, const QVariant & value);
        };
    }
}

#endif
