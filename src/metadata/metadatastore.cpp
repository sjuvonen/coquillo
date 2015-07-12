
#include "metadatastore.hpp"

namespace Coquillo {
    namespace MetaData {
        bool MetaDataStore::isModified(int row) const {
            return _backup.contains(_data[row].id());
        }

        bool MetaDataStore::isModified(const MetaData & item) const {
            return _backup.contains(item.id());
        }

        void MetaDataStore::rename(int row, const QString & new_path) {
            MetaData & meta = _data[row];
            if (backup(row)) {
                _only_path_changed << meta.id();
            }
            meta.setPath(new_path);
        }

        void MetaDataStore::update(int row, const MetaData & meta) {
            backup(row);
            _data.replace(row, meta);
            _only_path_changed.removeAll(meta.id());
        }

        void MetaDataStore::remove(int row) {
            const MetaData meta = at(row);
            if (isModified(meta)) {
                _backup.remove(meta.id());
                _only_path_changed.removeAll(meta.id());
            }
            _data.removeAt(row);

        }

        void MetaDataStore::restore(int row) {
            if (isModified(row)) {
                _data.replace(row, _backup[row]);
            }
        }

        bool MetaDataStore::backup(int row) {
            const MetaData meta = _data[row];
            if (!isModified(meta)) {
                _backup.insert(meta.id(), meta);
                return true;
            } else {
                return false;
            }
        }

        void MetaDataStore::reset() {
            for (int i = 0; i < _data.count(); i++) {
                const MetaData meta = _data[i];
                if (isModified(meta)) {
                    _data.replace(i, _backup[meta.id()]);
                }
            }
            _backup.clear();
            _only_path_changed.clear();
        }

        void MetaDataStore::clearBackup() {
            _backup.clear();
            _only_path_changed.clear();
        }

        const MetaData MetaDataStore::originalAt(int row) const {
            const MetaData item = _data[row];
            return _backup.contains(item.id()) ? _backup[item.id()] : item;
        }

        QHash<QString, QString> MetaDataStore::renamedPaths() const {
            QHash<QString, QString> paths;
            foreach (const MetaData meta, _data) {
                if (isModified(meta)) {
                    const MetaData orig = _backup[meta.id()];
                    if (meta.path() != orig.path()) {
                        paths.insert(orig.path(), meta.path());
                    }
                }
            }
            return paths;
        }

        QList<MetaData> MetaDataStore::modifiedExcludingOnlyRenamed() const {
            QList<MetaData> modified;
            modified.reserve(_backup.count() - _only_path_changed.count());
            foreach (const MetaData meta, _data) {
                if (isModified(meta) && !_only_path_changed.contains(meta.id())) {
                    modified << meta;
                }
            }
            return modified;
        }
    }
}
