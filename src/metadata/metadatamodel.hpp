#ifndef COQUILLO_METADATA_METADATAMODEL_H
#define COQUILLO_METADATA_METADATAMODEL_H

#include <QAbstractItemModel>
#include <QHash>
#include "metadata.hpp"
#include "metadatastore.hpp"

namespace Coquillo {
    namespace MetaData {
        class MediaCrawler;
        class MetaData;

        class MetaDataModel : public QAbstractItemModel {
            Q_OBJECT

            public:
                enum DataRole { FilePathRole = Qt::UserRole+1, FileNameRole = Qt::UserRole+2,
                    ModifiedRole, FieldNameRole, NamedRowDataRole, MetaDataRole, AudioPropertiesRole };

                MetaDataModel(QObject * parent = 0);
                int columnCount(const QModelIndex & idx = QModelIndex()) const;
                int rowCount(const QModelIndex & idx = QModelIndex()) const;
                QVariant data(const QModelIndex & idx, int role = Qt::DisplayRole) const;
                bool setData(const QModelIndex & idx, const QVariant & data, int role = Qt::EditRole);
                QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
                QModelIndex index(int row, int col, const QModelIndex & parent = QModelIndex()) const;
                bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());

                MetaData metaData(int row, bool original = false) const;
                MetaData metaData(const QModelIndex & idx, bool original = false) const { return metaData(idx.row(), original); }

                inline QModelIndex parent(const QModelIndex &) const {
                    return QModelIndex();
                }

                inline bool isRecursive() const { return _recursive; }
                inline QStringList directories() const { return _directories; }

            public slots:
                inline void setRecursive(bool state) { _recursive = state; }
                void addDirectory(const QString & path);
                void addFiles(const QStringList & files);
                void clear();
                void removeDirectory(const QString & path);
                void reload();
                void revert();
                void revert(const QModelIndex & idx);
                void writeToDisk();

            private slots:
                void addMetaData(const MetaData & metaData);
                void onWriteFinished();

            private:
                enum WorkerPriority { DirectoryReaderPriority, FileReaderPriority, FileWriterPriority };

                static QStringList nameFilters();
                bool isRowChanged(const QModelIndex & idx) const;
                bool isChanged(const QModelIndex & idx) const;
                void rowChanged(const QModelIndex & idx);
                QString containedDirectoryForRow(int row) const;

                bool _recursive;
                QHash<int, QString> _columns;
                QHash<int, QString> _columnMap;
                QStringList _directories;

                MetaDataStore _store;

        };
    }
}

#endif
