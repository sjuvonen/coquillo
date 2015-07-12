#ifndef COQUILLO_ITEMCOUNTLABEL_H
#define COQUILLO_ITEMCOUNTLABEL_H

#include <QLabel>
#include <QPointer>

class QAbstractItemModel;

namespace Coquillo {
    class ItemCountLabel : public QLabel {
        Q_OBJECT

        public:
            ItemCountLabel(QAbstractItemModel * model, QWidget * parent = 0);

        private slots:
            void onRowCountChanged();

        private:
            QPointer<QAbstractItemModel> _model;
    };
}


#endif
