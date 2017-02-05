
#include <QDebug>
#include "headerdatamodel.hpp"

namespace Coquillo {
    HeaderDataModel::HeaderDataModel(Qt::Orientation orientation, QObject * parent)
    : QAbstractListModel(parent), _orientation(orientation) {
    }

    int HeaderDataModel::columnCount(const QModelIndex & parent) const {
        if (parent.isValid() || !sourceModel()) {
            return 0;
        }
        return 1;
    }

    int HeaderDataModel::rowCount(const QModelIndex & parent) const {
        if (parent.isValid() || !sourceModel()) {
            return 0;
        }
        return sourceModel()->columnCount();
    }

    QVariant HeaderDataModel::data(const QModelIndex & idx, int role) const {
        if (!idx.isValid() || !sourceModel()) {
            return QVariant();
        }
        return sourceModel()->headerData(idx.row(), _orientation, role);
    }

    QVariant HeaderDataModel::headerData(int section, Qt::Orientation orientation, int role) const {
        bool valid_role = QList<int>({Qt::DisplayRole, Qt::EditRole}).contains(role);
        if (section == 0 && orientation == _orientation && valid_role) {
            return tr("Label");
        }
        return QVariant();
    }

    void HeaderDataModel::setSourceModel(QAbstractItemModel * model) {
        _model = model;
    }

    QAbstractItemModel * HeaderDataModel::sourceModel() const {
        return _model;
    }
}
