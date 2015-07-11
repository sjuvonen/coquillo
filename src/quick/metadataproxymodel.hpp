#ifndef COQUILLO_QUICK_METADATAPROXYMODEL_H
#define COQUILLO_QUICK_METADATAPROXYMODEL_H

#include <QSortFilterProxyModel>

namespace Coquillo {
    namespace Quick {
        class MetaDataProxyModel : public QSortFilterProxyModel {
            Q_OBJECT

            public:
                enum ColumnRoles { TitleRole = Qt::UserRole+1, FileNameRole };
                MetaDataProxyModel(QObject * parent = 0);

                int columnCount(const QModelIndex & =QModelIndex()) const;
                QHash<int, QByteArray> roleNames() const;

                QVariant data(const QModelIndex & idx, int role = Qt::DisplayRole) const;
        };
    }
}

#endif
