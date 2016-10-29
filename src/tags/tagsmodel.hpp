#ifndef COQUILLO_TAGS_TAGSMODEL_H
#define COQUILLO_TAGS_TAGSMODEL_H

#include <QAbstractItemModel>
#include "tagstore.hpp"

namespace Coquillo {
    namespace Tags {
        class TagsModel : public QAbstractItemModel {
            Q_OBJECT

            public:
                enum Fields { PathField = 14, ImageField = 15 };

                TagsModel(QObject * parent = 0);

                int columnCount(const QModelIndex & parent = QModelIndex()) const;
                int rowCount(const QModelIndex & parent = QModelIndex()) const;
                QVariant data(const QModelIndex & idx, int role = Qt::DisplayRole) const;
                QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
                QModelIndex index(int row, int col, const QModelIndex & parent = QModelIndex()) const;
                bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());
                bool setData(const QModelIndex & idx, const QVariant & value, int role = Qt::EditRole);

                inline QModelIndex parent(const QModelIndex &) const {
                    return QModelIndex();
                }

            public slots:
                void addPath(const QString & path);
                void addPaths(const QStringList & paths);
                void removeDirectory(const QString & path);
                void setRecursive(bool state) { _recursive = state; }

            private:
                const Container tagContainer(const QModelIndex & idx) const;
                const Container tagContainer(int row) const;
                QString containedDirectoryForRow(int row) const;
                void rowChanged(const QModelIndex & idx);

                QHash<int, QString> _columns;
                QHash<int, QString> _columnMap;
                Store _store;
                bool _recursive;
                QStringList _directories;
        };
    }
}

#endif
