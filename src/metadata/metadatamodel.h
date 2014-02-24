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
            enum DataRole { ModifiedRole = Qt::UserRole+1, FieldNameRole, NamedRowDataRole };

            MetaDataModel(QObject * parent = 0);
            ~MetaDataModel();
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

        private slots:
            void addMetaData(const MetaData & metaData);

        private:
            static QStringList nameFilters();
            void backup(const MetaData & metaData);
            bool isRowChanged(const QModelIndex & idx) const;
            bool isChanged(const QModelIndex & idx) const;
            void rowChanged(const QModelIndex & idx);
            QString containedDirectoryForRow(int row) const;
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
