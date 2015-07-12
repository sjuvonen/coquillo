
#include <QAbstractItemModel>
#include "itemcountlabel.hpp"

namespace Coquillo {
    ItemCountLabel::ItemCountLabel(QAbstractItemModel * model, QWidget * parent)
    : QLabel(parent), _model(model) {
        if (model) {
            connect(model, SIGNAL(rowsInserted(QModelIndex, int, int)), SLOT(onRowCountChanged()));
            connect(model, SIGNAL(rowsRemoved(QModelIndex, int, int)), SLOT(onRowCountChanged()));
        }
    }

    void ItemCountLabel::onRowCountChanged() {
        setText(QString("%1 files").arg(_model->rowCount()));
    }
}
