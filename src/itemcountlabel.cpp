
#include <QAbstractItemModel>
#include "itemcountlabel.hpp"

namespace Coquillo {
    ItemCountLabel::ItemCountLabel(QAbstractItemModel * model, QWidget * parent)
    : QLabel(parent), _model(model) {
        if (model) {
            connect(model, SIGNAL(rowsInserted(QModelIndex, int, int)), SLOT(onRowCountChanged()));
            connect(model, SIGNAL(rowsRemoved(QModelIndex, int, int)), SLOT(onRowCountChanged()));
        }
        onRowCountChanged();
    }

    void ItemCountLabel::onRowCountChanged() {
        if (_model->rowCount() > 0) {
            setText(QString("%1 files").arg(_model->rowCount()));
        } else {
            setText(tr("No files"));
        }
    }
}
