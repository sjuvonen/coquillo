
#include "selectionfiltermodel.hpp"

#include <QDebug>

namespace Coquillo {
    namespace WebTags {
        void SelectionFilterModel::setSelection(const QModelIndexList & indices) {
            QList<QPersistentModelIndex> copy;

            foreach (const QModelIndex & idx, indices) {
                copy << QPersistentModelIndex(idx);
            }

            setSelection(copy);
        }

        void SelectionFilterModel::setSelection(const QList<QPersistentModelIndex> & indices) {
            _selection = indices;
            invalidateFilter();
        }

        bool SelectionFilterModel::filterAcceptsColumn(int column, const QModelIndex & parent) const {
            foreach (const QPersistentModelIndex & idx, _selection) {
                if (idx.column() == column && idx.parent() == parent) {
                    return true;
                }
            }
            return false;
        }

        bool SelectionFilterModel::filterAcceptsRow(int row, const QModelIndex & parent) const {
            foreach (const QPersistentModelIndex & idx, _selection) {
                if (idx.row() == row && idx.parent() == parent) {
                    return true;
                }
            }
            return false;
        }

        // int SelectionFilterModel::columnCount(const QModelIndex & parent) const {
        //     int count = QSortFilterProxyModel::columnCount(parent);
        //     return count ? count + 1 : 0;
        // }
        //
        // QVariant SelectionFilterModel::data(const QModelIndex & idx, int role) const {
        //     if (!isCustomColumn(idx)) {
        //         return QSortFilterProxyModel::data(idx, role);
        //     }
        //     qDebug() << "DATA";
        //
        //     switch (role) {
        //         case Qt::CheckStateRole:
        //             return Qt::Unchecked;
        //
        //         case Qt::DisplayRole:
        //         case Qt::EditRole:
        //             return "foo";
        //
        //         default:
        //             return QVariant();
        //     }
        // }
        //
        // Qt::ItemFlags SelectionFilterModel::flags(const QModelIndex & idx) const {
        //     if (!isCustomColumn(idx)) {
        //         return QSortFilterProxyModel::flags(idx);
        //     }
        //     return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemNeverHasChildren;
        // }
        //
        // QVariant SelectionFilterModel::headerData(int section, Qt::Orientation orientation, int role) const {
        //     if (orientation != Qt::Horizontal || role != Qt::DisplayRole || !isCustomColumn(section)) {
        //         return QSortFilterProxyModel::headerData(section, orientation, role);
        //     } else {
        //         return role == Qt::DisplayRole ? QVariant(QString("Label")) : QVariant();
        //     }
        // }
        //
        // bool SelectionFilterModel::isCustomColumn(const QModelIndex & idx) const {
        //     qDebug() << "colsize" << idx.column() << QSortFilterProxyModel::columnCount(idx.parent());
        //     return idx.column() >= QSortFilterProxyModel::columnCount(idx.parent());
        // }
        //
        // bool SelectionFilterModel::isCustomColumn(int column) const {
        //     return column >= QSortFilterProxyModel::columnCount();
        // }

        // QModelIndex SelectionFilterModel::mapFromSource(const QModelIndex & src_idx) const {
        //     // qDebug() << "mfs";
        //     const QModelIndex mapped = QSortFilterProxyModel::mapFromSource(src_idx);
        //     return mapped;
        //
        //     if (mapped.isValid()) {
        //         return createIndex(mapped.row(), mapped.column() + 1, mapped.internalId());
        //     } else {
        //         return mapped;
        //     }
        // }
        //
        // QModelIndex SelectionFilterModel::mapToSource(const QModelIndex & idx) const {
        //     const QModelIndex src_idx = QSortFilterProxyModel::mapToSource(idx);
        //     return src_idx;
        //
        //     if (src_idx.isValid()) {
        //         qDebug() << "mts" << src_idx;
        //         return sourceModel()->index(src_idx.row(), src_idx.column() - 1, src_idx.parent());
        //     } else {
        //         return src_idx;
        //     }
        // }

        // QModelIndex SelectionFilterModel::index(int row, int column, const QModelIndex & parent) const {
        //     if (!isCustomColumn(column)) {
        //         return QSortFilterProxyModel::index(row, column, parent);
        //     } else {
        //         return QModelIndex();
        //         // return createIndex(row, 0, row * 1000000 + column);
        //     }
        // }
    }
}
