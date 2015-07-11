
#include <QDebug>

#include "metadataproxymodel.hpp"

namespace Coquillo {
    namespace Quick {
        MetaDataProxyModel::MetaDataProxyModel(QObject * parent)
        : QSortFilterProxyModel(parent) {

        }

        int MetaDataProxyModel::columnCount(const QModelIndex & parent) const {
            if (parent.isValid()) {
                return 0;
            }

            return 2;
        }

        QHash<int, QByteArray> MetaDataProxyModel::roleNames() const {
            QHash<int, QByteArray> roles;
            roles[TitleRole] = "title";
            roles[FileNameRole] = "filename";
            return roles;
        }

        QVariant MetaDataProxyModel::data(const QModelIndex & idx, int role) const {
            const QModelIndex src_idx = mapToSource(idx);
            const int row = src_idx.row();

            if (role == TitleRole) {
                return src_idx.sibling(row, 1).data();
            }

            if (role == FileNameRole) {
                return src_idx.sibling(row, 15).data();
            }

            return QVariant();
        }
    }
}
