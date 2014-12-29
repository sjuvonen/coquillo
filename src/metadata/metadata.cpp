
#include <QDebug>

#include "mapper.h"
#include "metadata.h"

namespace Coquillo {
    namespace MetaData {

        void MetaData::registerMapper(const QString & tag, Mapper * mapper) {
            mappers[tag] = mapper;
        }

        Mapper * MetaData::getMapper(const QString & tag) {
            if (mappers.contains(tag)) {
                return mappers.value(tag);
            } else {
                return mappers.value("unknown");
            }
        }

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

        bool MetaData::hasTag(const QString & name) const {
            return _tags.contains(name);
        }

        bool MetaData::has(const QString & key) const {
            return has(key, _primary);
        }

        bool MetaData::has(const QString & key, const QString & tag) const {
            return hasTag(tag) and _tags[tag].contains(key);
        }

        void MetaData::insert(const QString & key, const QVariant & value) {
            foreach (const QString tag, _tags.keys()) {
                getMapper(tag)->insert(_tags[tag], key, value);
            }
        }

        QVariant MetaData::value(const QString & key, const QString & tag) const {
            return getMapper(tag)->value(_tags[tag], key);
        }

        QStringList MetaData::fields() const {
            return _tags[_primary].keys();
        }

        QStringList MetaData::fields(const QString & tag) const {
            return hasTag(tag) ? _tags.value(tag).keys() : QStringList();
        }
    }
}
