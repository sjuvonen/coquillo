#include <QIcon>
#include <QModelIndex>
#include "tags/tagdataroles.hpp"
#include "itemmodifiedproxy.hpp"

#include <QDebug>

namespace Coquillo {
    ItemModifiedProxy::ItemModifiedProxy(QObject * parent)
    : QIdentityProxyModel(parent) {

    }

    QModelIndex ItemModifiedProxy::mapFromSource(const QModelIndex & idx) const {
        return createIndex(idx.row(), idx.column() + 1, idx.internalId());
    }

    QModelIndex ItemModifiedProxy::mapToSource(const QModelIndex & idx) const {
        if (idx.column() == 0) {
            return QModelIndex();
        } else {
            return sourceModel()->index(idx.row(), idx.column() - 1);
        }
    }

    int ItemModifiedProxy::columnCount(const QModelIndex & parent) const {
        int count = QIdentityProxyModel::columnCount(parent);
        return count > 0 ? count + 1 : 0;
    }

    QVariant ItemModifiedProxy::data(const QModelIndex & idx, int role) const {
        qDebug() << idx;
        if (idx.column() == 0 && role == Qt::DecorationRole) {
            bool modified = index(idx.row(), 1).data(Tags::ItemModifiedStateRole).toBool();
            if (modified) {
                return QIcon::fromTheme("emblem-important");
            }
        }
        return QIdentityProxyModel::data(idx, role);
    }
}
