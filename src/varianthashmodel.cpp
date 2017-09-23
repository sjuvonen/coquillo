
#include "varianthashmodel.hpp"

namespace Coquillo {
    VariantHashModel::VariantHashModel(QObject * parent)
    : QAbstractTableModel(parent) {

    }

    VariantHashModel::VariantHashModel(const QList<QVariantMap> & data, QObject * parent)
    : QAbstractTableModel(parent), _data(data) {

    }

    int VariantHashModel::rowCount(const QModelIndex & parent) const {
        if (parent.isValid()) {
            return 0;
        }
        return _data.size();
    }

    int VariantHashModel::columnCount(const QModelIndex & parent) const {
        if (parent.isValid()) {
            return 0;
        }
        return _labels.size();
    }

    QVariant VariantHashModel::data(const QModelIndex & idx, int role) const {
        if (!idx.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole)) {
            return QVariant();
        }

        int row = idx.row();
        const QString col = _keys[idx.column()];

        return _data[row][col];
    }

    void VariantHashModel::setData(const QList<QVariantMap> & data) {
        beginResetModel();
        _data = data;
        endResetModel();
    }

    QVariant VariantHashModel::headerData(int section, Qt::Orientation orientation, int role) const {
        if (orientation == Qt::Vertical) {
            return QAbstractTableModel::headerData(section, orientation, role);
        }
        if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
            return _labels[section];
        }
        return QVariant();
    }

    void VariantHashModel::setKeys(const QStringList & keys) {
        beginResetModel();
        _keys = keys;
        endResetModel();
    }

    void VariantHashModel::setHeaderLabels(const QStringList & labels) {
        _labels = labels;
        emit headerDataChanged(Qt::Horizontal, 0, columnCount());
    }
}
