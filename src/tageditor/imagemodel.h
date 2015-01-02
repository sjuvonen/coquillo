#ifndef COQUILLO_TAGEDITOR_IMAGEMODEL_H
#define COQUILLO_TAGEDITOR_IMAGEMODEL_H

#include <QAbstractTableModel>
#include <QPointer>

namespace Coquillo {
    namespace MetaData {
        class MetaData;
    }

    namespace TagEditor {
        class ImageModel : public QAbstractTableModel {
            Q_OBJECT

            public:
                ImageModel(QObject * parent = 0);
                void addImage(const QImage & image);
                MetaData::MetaData metaData() const;

                QVariant data(const QModelIndex & idx, int role=Qt::DisplayRole) const;
                QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
                bool removeRows(int row, int count, const QModelIndex & p=QModelIndex());
                int rowCount(const QModelIndex & p = QModelIndex()) const;
                bool setData(const QModelIndex & idx, const QVariant & value, int role = Qt::EditRole);

                inline void setSourceModel(QAbstractItemModel * model) { _source = model; }
                inline QAbstractItemModel * sourceModel() const { return _source; }
                inline int columnCount(const QModelIndex & = QModelIndex()) const { return 4; }
                inline QModelIndex sourceIndex() const { return _index; }

            public slots:
                void setSourceIndex(const QModelIndex & idx);

            private:
                QPointer<QAbstractItemModel> _source;
                QPersistentModelIndex _index;
        };
    }
}

#endif
