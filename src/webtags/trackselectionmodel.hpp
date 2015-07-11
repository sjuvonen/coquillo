#ifndef COQUILLO_WEBTAGS_TRACKSELECTIONMODEL_H
#define COQUILLO_WEBTAGS_TRACKSELECTIONMODEL_H

#include <QPersistentModelIndex>
#include <QSortFilterProxyModel>

namespace Coquillo {
    namespace WebTags {
        class TrackSelectionModel : public QSortFilterProxyModel {
            Q_OBJECT

            public:
                TrackSelectionModel(QObject * parent = 0);
                void shiftRows(const QModelIndexList & indices, int delta);

                QVariant data(const QModelIndex & idx, int role = Qt::DisplayRole) const;
                Qt::ItemFlags flags(const QModelIndex & idx) const;
                QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
                bool setData(const QModelIndex & idx, const QVariant & value, int role = Qt::EditRole);
                void setSourceModel(QAbstractItemModel * model);

            protected:
                bool lessThan(const QModelIndex & left, const QModelIndex & right) const;

                inline bool filterAcceptsColumn(int src_col, const QModelIndex &) const {
                    return src_col == CheckableColumn;
                }

            private:
                void shiftRow(const QModelIndex & idx, int delta, bool sort_again);
                QList<int> _unchecked;
                QList<int> _sorted;
                static const int CheckableColumn;
        };
    }
}

#endif
