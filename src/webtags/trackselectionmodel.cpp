
#include <QDebug>
#include "trackselectionmodel.hpp"

namespace Coquillo {
    namespace WebTags {
        const int TrackSelectionModel::CheckableColumn = 15;

        TrackSelectionModel::TrackSelectionModel(QObject * parent)
        : QSortFilterProxyModel(parent) {

        }

        QVariant TrackSelectionModel::data(const QModelIndex & idx, int role) const {
            if (role == Qt::CheckStateRole and idx.column() == CheckableColumn) {
               return _unchecked.contains(mapToSource(idx).row()) ? Qt::Unchecked : Qt::Checked;
            } else {
                return QSortFilterProxyModel::data(idx, role);
            }
        }

        Qt::ItemFlags TrackSelectionModel::flags(const QModelIndex &) const {
            return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
        }

        QVariant TrackSelectionModel::headerData(int section, Qt::Orientation orientation, int role) const {
            if (orientation == Qt::Vertical and role == Qt::DisplayRole) {
                return section+1;
            } else {
                return QSortFilterProxyModel::headerData(section, orientation, role);
            }
        }

        bool TrackSelectionModel::lessThan(const QModelIndex & left, const QModelIndex & right) const {
            return _sorted.indexOf(left.row()) < _sorted.indexOf(right.row());
        }

        bool TrackSelectionModel::setData(const QModelIndex & idx, const QVariant & value, int role) {
            if (role == Qt::CheckStateRole and idx.column() == CheckableColumn) {
                const int row = mapToSource(idx).row();
                if (!value.toBool()) {
                    if (!_unchecked.contains(row)) {
                        _unchecked << row;
                    }
                } else {
                    _unchecked.removeAll(row);
                }
                return true;
            } else {
                return QSortFilterProxyModel::setData(idx, value, role);
            }
        }

        void TrackSelectionModel::setSourceModel(QAbstractItemModel * model) {
            QSortFilterProxyModel::setSourceModel(model);
            for (int i = 0; i <= model->rowCount(); i++) {
                _sorted << i;
            }
            sort(0, Qt::AscendingOrder);
        }

        void TrackSelectionModel::shiftRows(const QModelIndexList & indices, int delta) {
            if (delta < 0) {
                for (int i = indices.count()-1; i >= 0; i--) {
                    shiftRow(indices[i], delta, false);
                }
            } else if (delta > 0) {
                for (int i = indices.count()-1; i >= 0; i--) {
                    shiftRow(indices[i], delta, false);
                }
            }
            invalidate();
        }

        void TrackSelectionModel::shiftRow(const QModelIndex & idx, int delta, bool sort_again) {
            if (idx.isValid()) {
                int from = idx.row();
                int to = qMax(qMin(from+delta, _sorted.size()-1), 0);

                _sorted.move(from, to);

                if (sort_again) {
                    invalidate();
                }
            }
        }
    }
}
