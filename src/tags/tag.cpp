
#include "tag.hpp"

#include <QDebug>

namespace Coquillo {
    namespace Tags {
        Tag::Tag(const QString & type, const Mapping & mapping, const QVariantHash & values)
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
            const QString mapped = _mapping.value(field);

            if (mapped.isNull()) {
                return false;
            }

            if (_raw.contains(mapped)) {
                const QVariant old = _raw.value(mapped);
                bool int_changed = value.type() == QVariant::Int && (value.toInt() != old.toInt());
                bool str_changed = value.type() != QVariant::Int && (value.toString() != old.toString());

                if (!int_changed && !str_changed) {
                    return false;
                }
            } else {
                if (value.isNull() || !value.isValid()) {
                    return false;
                }
            }

            qDebug() << "set" << id() << field << value.toString() << _raw.value(mapped).toString();

            _raw.insert(mapped, value);
            return true;
        }

        QVariantList Tag::all(const QString & raw) const {
            return _raw.values(raw);
        }

        bool Tag::equals(const QString & field, const QVariant & value) const {
            const QString mapped = _mapping.value(field);
            const QVariant old = _raw.value(mapped);
            bool int_changed = value.type() == QVariant::Int && (value.toInt() != old.toInt());
            bool str_changed = value.type() != QVariant::Int && (value.toString() != old.toString());

            return !int_changed && !str_changed;
        }
    }
}
