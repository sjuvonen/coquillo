#ifndef COQUILLO_ITEMMODIFIEDPROXY_H
#define COQUILLO_ITEMMODIFIEDPROXY_H

#include <QIdentityProxyModel>

namespace Coquillo {
    class ItemModifiedProxy : public QIdentityProxyModel {
        Q_OBJECT

        public:
            ItemModifiedProxy(QObject * parent = nullptr);
            QModelIndex mapFromSource(const QModelIndex & idx) const;
            QModelIndex mapToSource(const QModelIndex & idx) const;
            QVariant data(const QModelIndex & idx, int role = Qt::DisplayRole) const;
            int columnCount(const QModelIndex & parent = QModelIndex()) const;
    };
}

#endif
