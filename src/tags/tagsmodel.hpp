#ifndef COQUILLO_TAGS_TAGSMODEL_H
#define COQUILLO_TAGS_TAGSMODEL_H

#include <QAbstractItemModel>
#include "tagstore.hpp"

namespace Coquillo {
    namespace Tags {
        class TagsModel : public QAbstractItemModel {
            Q_OBJECT

            public:
                TagsModel(QObject * parent = 0);

                int columnCount(const QModelIndex & parent = QModelIndex()) const;
                int rowCount(const QModelIndex & parent = QModelIndex()) const;
                QVariant data(const QModelIndex & idx, int role = Qt::DisplayRole) const;
                QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
                QModelIndex index(int row, int col, const QModelIndex & parent = QModelIndex()) const;

                inline QModelIndex parent(const QModelIndex &) const {
                    return QModelIndex();
                }

            private:
                QHash<int, QString> _columns;
                QHash<int, QString> _columnMap;
                Store _store;
        };
    }
}

#endif
