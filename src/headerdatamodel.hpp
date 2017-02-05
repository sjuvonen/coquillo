#ifndef COQUILLO_HEADERDATAMODEL_H
#define COQUILLO_HEADERDATAMODEL_H

#include <QAbstractListModel>
#include <QPointer>

namespace Coquillo {
    class HeaderDataModel : public QAbstractListModel {
        Q_OBJECT

        public:
            HeaderDataModel(Qt::Orientation orientation, QObject * parent = nullptr);

            int columnCount(const QModelIndex & parent = QModelIndex()) const;
            int rowCount(const QModelIndex & parent = QModelIndex()) const;
            QVariant data(const QModelIndex & idx, int role = Qt::DisplayRole) const;
            QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

            void setSourceModel(QAbstractItemModel * model);
            QAbstractItemModel * sourceModel() const;

        private:
            Qt::Orientation _orientation;
            QPointer<QAbstractItemModel> _model;
    };
}

#endif
