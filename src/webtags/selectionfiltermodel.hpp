#ifndef COQUILLO_WEBTAGS_SELECTIONFILTERMODEL_H
#define COQUILLO_WEBTAGS_SELECTIONFILTERMODEL_H

#include <QSortFilterProxyModel>

namespace Coquillo {
    namespace WebTags {
        class SelectionFilterModel : public QSortFilterProxyModel {
            Q_OBJECT

            public:
                void setSelection(const QModelIndexList & indices);
                void setSelection(const QList<QPersistentModelIndex> & indices);

                /*
                 * NOTE: It is waste of time to try to hack QSortFilterProxyModel in order
                 * to enable virtual columns. Next time implement this proxy from scratch.
                 */

                // int columnCount(const QModelIndex & parent = QModelIndex()) const;
                // QVariant data(const QModelIndex & idx, int role = Qt::DisplayRole) const;
                // QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
                // Qt::ItemFlags flags(const QModelIndex & idx) const;
                //
                //
                // // QModelIndex mapFromSource(const QModelIndex & idx) const;
                // // QModelIndex mapToSource(const QModelIndex & idx) const;
                // QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;


            protected:
                bool filterAcceptsColumn(int column, const QModelIndex & parent) const;
                bool filterAcceptsRow(int row, const QModelIndex & parent) const;

            private:
                // bool isCustomColumn(const QModelIndex & idx) const;
                // bool isCustomColumn(int column) const;
                QList<QPersistentModelIndex> _selection;
        };
    }
}

#endif
