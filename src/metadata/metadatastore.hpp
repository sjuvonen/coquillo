#ifndef COQUILLO_METADATA_METADATASTORE_H
#define COQUILLO_METADATA_METADATASTORE_H

#include <QHash>
#include <QList>
#include "metadata.hpp"

namespace Coquillo {
    namespace MetaData {
        class MetaDataStore {
            public:
                MetaDataStore() { }
                const QList<MetaData> & items() const { return _data; }
                const MetaData at(int row) const { return _data.at(row); }
                MetaData & at(int row) { return _data[row]; }
                int count() const { return _data.count(); }
                void append(const MetaData & meta) { _data << meta; }
                void append(const QList<MetaData> & meta) { _data << meta; }
                void clear() { _data.clear(); _backup.clear(); _only_path_changed.clear(); }

                bool isModified(int row) const;
                bool isModified(const MetaData & item) const;
                void rename(int row, const QString & new_path);
                void update(int row, const MetaData & data);
                void remove(int row);
                void restore(int row);
                bool backup(int row);
                void reset();
                void clearBackup();

                const MetaData originalAt(int row) const;

                QHash<QString, QString> renamedPaths() const;
                QList<MetaData> modifiedExcludingOnlyRenamed() const;


            private:
                QList<MetaData> _data;
                QHash<uint, MetaData> _backup;
                QList<uint> _only_path_changed;
        };
    }
}

#endif
