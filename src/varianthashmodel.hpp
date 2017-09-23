#ifndef COQUILLO_VARIANTHASHMODEL_H
#define COQUILLO_VARIANTHASHMODEL_H

#include <QAbstractTableModel>

namespace Coquillo {
    class VariantHashModel : public QAbstractTableModel {
        Q_OBJECT

        public:
            enum ItemDataRole { RowDataRole = Qt::UserRole + 1 };
            
            VariantHashModel(QObject * parent = nullptr);
            VariantHashModel(const QList<QVariantMap> & data, QObject * parent = nullptr);

            int rowCount(const QModelIndex & parent = QModelIndex()) const;
            int columnCount(const QModelIndex & parent = QModelIndex()) const;
            QVariant data(const QModelIndex & idx, int role = Qt::EditRole) const;
            QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

            void setData(const QList<QVariantMap> & data);
            void setKeys(const QStringList & keys);
            void setHeaderLabels(const QStringList & labels);

        private:
            QStringList _labels;
            QStringList _keys;
            QList<QVariantMap> _data;
    };
}

#endif
