#include "tagcontainer.hpp"

namespace Coquillo {
    namespace Tags {
        Container::Container()
        : _id(0) {

        }

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

        bool Container::hasTag(const QString & id) const {
            return _tags.contains(id);
        }

        const Tag Container::tag(const QString & id) const {
            return _tags[id];
        }

        Tag & Container::tag(const QString & id) {
            return _tags[id];
        }

        QList<Tag> Container::tags() const {
            return _tags.values();
        }

        QStringList Container::tagNames() const {
            return _tags.keys();
        }
    }
}
