
#include <QDebug>

#include "mapper.h"
#include "metadata.h"

namespace Coquillo {
    namespace MetaData {

        void MetaData::registerMapper(const QString & tag, Mapper * mapper) {
            mappers[tag] = mapper;
        }

        Mapper * MetaData::getMapper(const QString & tag) {
            return mappers.value(tag);
        }

        void MetaData::addTag(const QString & name, const Tag & tag) {
            _tags[name] = Tag();

            if (_primary.isNull()) {
                setPrimaryTag(name);
            }

            if (Mapper * mapper = getMapper(name)) {
                foreach (const QString key, tag.keys()) {
    //                 insert(name, tag[key]);
                    mapper->insert(_tags[name], key, tag[key]);
                }
            }

            qDebug() << this->tag(name);
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
                Mapper * mapper = getMapper(tag);
                if (mapper) {
                    mapper->insert(_tags[tag], key, value);
                }
            }
        }

        QVariant MetaData::value(const QString & key, const QString & tag) const {
            if (Mapper * mapper = getMapper(tag)) {
                return mapper->value(_tags[tag], key);
            } else {
                return QVariant();
            }
        }

        QStringList MetaData::fields() const {
            return _tags[_primary].keys();
        }

        QStringList MetaData::fields(const QString & tag) const {
            return hasTag(tag) ? _tags.value(tag).keys() : QStringList();
        }
    }
}
