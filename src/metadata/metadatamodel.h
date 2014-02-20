#ifndef COQUILLO_METADATAMODEL_H
#define COQUILLO_METADATAMODEL_H

#include <QAbstractItemModel>
#include <QHash>
#include "metadata.h"

class QThread;

namespace Coquillo {
    class MediaCrawler;
    class MetaData;

    class MetaDataModel : public QAbstractItemModel {
        Q_OBJECT

        public:
            enum DataRole { ModifiedRole = Qt::UserRole+1, FieldNameRole };

            MetaDataModel(QObject * parent = 0);
            ~MetaDataModel();
            int columnCount(const QModelIndex & idx = QModelIndex()) const;
            int rowCount(const QModelIndex & idx = QModelIndex()) const;
            QVariant data(const QModelIndex & idx, int role = Qt::DisplayRole) const;
            bool setData(const QModelIndex & idx, const QVariant & data, int role = Qt::EditRole);
            QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
            QModelIndex index(int row, int col, const QModelIndex & parent) const;
            bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());

            MetaData metaData(int row) const { return _metaData.value(row); }
            MetaData metaData(const QModelIndex & idx) const { return metaData(idx.row()); }

            inline QModelIndex parent(const QModelIndex &) const {
                return QModelIndex();
            }

            inline bool isRecursive() const { return _recursive; }

        public slots:
            void addDirectory(const QString & path);
            void removeDirectory(const QString & path);
            void addFiles(const QStringList & files);
            inline void setRecursive(bool state) { _recursive = state; }

        private slots:
            void addMetaData(const MetaData & metaData);

        private:
            static QStringList nameFilters();
            void backup(const MetaData & metaData);
            bool isRowChanged(const QModelIndex & idx) const;
            bool isChanged(const QModelIndex & idx) const;
//             bool rowBelongsToPath(const QModelIndex & idx, const QString & dir) const;
            QString containedDirectoryForRow(int row) const;
            QStringList scanDirectory(const QString & path);
            MediaCrawler * createCrawler();
            QThread * createWorker();

            bool _recursive;
            QHash<int, QString> _columns;
            QHash<int, QString> _columnMap;
            QList<QThread *> _workers;
            QList<MetaData> _metaData;
            QHash<QString, MetaData> _original;
            QStringList _directories;
    };
}

#endif
