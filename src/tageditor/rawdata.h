#ifndef COQUILLO_TAGEDITOR_RAWDATA_H
#define COQUILLO_TAGEDITOR_RAWDATA_H

#include <QPersistentModelIndex>
#include <QPointer>
#include <QWidget>

class QStandardItemModel;

namespace Ui {
    class RawData;
}

namespace Coquillo {
    namespace MetaData {
        class MetaData;
    }

    namespace TagEditor {
        class RawData : public QWidget {
            Q_OBJECT

            public:
                RawData(QWidget * parent = 0);
                ~RawData();

                void setModel(QAbstractItemModel * model);
                inline QAbstractItemModel * model() const { return _model; }

            public slots:
                void setCurrentIndex(const QModelIndex & idx);

            private slots:
                void selectTag(const QString & name);

            private:
                MetaData::MetaData metaData() const;
                QStandardItemModel * treeModel() const;
                Ui::RawData * _ui;
                QPointer<QAbstractItemModel> _model;
                QPersistentModelIndex _current;
        };
    }
}

#endif
