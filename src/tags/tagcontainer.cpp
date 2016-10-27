#include "tagcontainer.hpp"

namespace Coquillo {
    namespace Tags {
        Container::Container(const QString & path)
        : _id(qHash(path)), _path(path) {

        }

        void Container::addTag(const Tag & tag) {
            _tags.insert(tag.id(), tag);

            if (_primary.isNull()) {
                _primary = tag.id();
            }
        }

        void Container::setPrimaryTag(const QString & id) {
            _primary = id;
        }

        QVariant Container::value(const QString & field) const {
            return _tags[_primary].value(field);
        }
    }
}
