
#include <QItemSelectionModel>
#include "selectionproxymodel.hpp"

namespace Coquillo {
    SelectionProxyModel::SelectionProxyModel(QItemSelectionModel * selection, QObject * parent)
    : QSortFilterProxyModel(parent) {
        setSelectionModel(selection);
    }

    void SelectionProxyModel::setSelectionModel(QItemSelectionModel * model) {
        _selection = model;
        setSourceModel(model->model());
    }

    bool SelectionProxyModel::filterAcceptsRow(int source_row, const QModelIndex & source_parent) const {
        if (!_selection) {
            return false;
        }

        if (QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent)) {
            const QModelIndexList rows(_selection->selectedRows());

            for (const auto & idx : rows) {
                if (idx.row() == source_row) {
                    return true;
                }
            }
        }

        return false;
    }
}
