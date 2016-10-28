
#include "tag.hpp"

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

        QVariantList Tag::all(const QString & raw) const {
            return _raw.values(raw);
        }
    }
}
