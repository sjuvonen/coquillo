
#include "tag.hpp"

namespace Coquillo {
    namespace Tags {
        Tag::Tag(const QString & type, const Mapping & mapping, const QVariantHash & values)
        : _id(type), _mapping(mapping), _raw(values) {

        }

        QVariant Tag::value(const QString & field) const {
            const QString mapped = _mapping.value(field);
            return mapped.isNull() ? QVariant() : _raw.value(mapped);
        }
    }
}
