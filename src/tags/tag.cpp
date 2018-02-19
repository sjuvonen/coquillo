
#include "tag.hpp"

#include <QDebug>

namespace Coquillo {
    namespace Tags {
        Tag::Tag(const QString & type, const Mapping & mapping, const QVariantMap & values)
        : _id(type), _mapping(mapping), _raw(values) {

        }

        QStringList Tag::keys() const {
            QStringList all = _raw.keys();
            all.removeDuplicates();
            return all;
        }

        QVariant Tag::value(const QString & field) const {
            const QString mapped = _mapping.value(field);
            return mapped.isNull() ? QVariant() : _raw.value(mapped);
        }

        bool Tag::insert(const QString & field, const QVariant & value) {
            if (equals(field, value)) {
                return false;
            }

            const QString mapped = _mapping.value(field);

            // qDebug() << "set" << id() << field << _raw.value(mapped).toString() << value.toString();
            _raw.insert(mapped, value);
            return true;
        }

        QVariantList Tag::all(const QString & raw) const {
            return _raw.values(raw);
        }

        bool Tag::equals(const QString & field, const QVariant & value) const {
            if (!_mapping.contains(field)) {
                return true;
            }

            const QString mapped = _mapping.value(field);
            const QVariant old = _raw.value(mapped);
            const QList<QVariant::Type> number_types = {QVariant::Int, QVariant::UInt};

            if (number_types.contains(old.type()) || number_types.contains(value.type())) {
                return value.toInt() == old.toInt();
            } else {
                return value.toString() == old.toString();
            }
        }
    }
}
