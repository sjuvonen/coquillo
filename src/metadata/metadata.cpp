
#include "metadata.h"

namespace Coquillo {
    void MetaData::addTag(const QString & name, const Tag & tag) {
        _tags[name] = tag;

        if (_primary.isNull()) {
            setPrimaryTag(name);
        }
    }

    void MetaData::removeTag(const QString & name) {
        _tags.remove(name);
    }

    void MetaData::setPrimaryTag(const QString & name) {
        if (hasTag(name)) {
            _primary = name;
        }
    }

    bool MetaData::hasTag(const QString & name) {
        return _tags.contains(name);
    }

    bool MetaData::has(const QString & key) {
        return has(key, _primary);
    }

    bool MetaData::has(const QString & key, const QString & tag) {
        if (hasTag(tag)) {
            return _tags[tag].contains(key);
        } else {
            return false;
        }
    }

    void MetaData::insert(const QString & key, const QVariant & value) {
        _tags[_primary][key] = value;
    }

    QVariant MetaData::value(const QString & key, const QString & tag) const {
        return _tags.value(tag).value(key);
    }
}
