#ifndef COQUILLO_TAGEDITOR_IMAGEMODEL_H
#define COQUILLO_TAGEDITOR_IMAGEMODEL_H

#include <QAbstractTableModel>

namespace Coquillo {
    namespace MetaData {
        class MetaData;
    }

    namespace TagEditor {
        class ImageModel : public QAbstractTableModel {
            Q_OBJECT

            public:
                ImageModel(QObject * parent = 0);
                QVariant data(const QModelIndex & idx, int role=Qt::DisplayRole) const;
                QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
                int rowCount(const QModelIndex & p = QModelIndex()) const;
                inline int columnCount(const QModelIndex & = QModelIndex()) const { return 4; }
                QModelIndex sourceIndex() const { return _source; }

            public slots:
                void setSourceIndex(const QModelIndex & idx);

            private:
                MetaData::MetaData metaData() const;
                QPersistentModelIndex _source;
        };
    }
}

#endif
