
#include <QSortFilterProxyModel>

#include "ui_sortpicker.h"
#include "sortpicker.hpp"

namespace Coquillo {
    SortPicker::SortPicker(QWidget * parent)
    : QWidget(parent) {
        _ui = new Ui::SortPicker;
        _ui->setupUi(this);

        _proxy = new QSortFilterProxyModel(this);
        _ui->comboBox->setModel(_proxy);
    }

    void SortPicker::setModel(QAbstractItemModel * model) {
        _proxy->setSourceModel(model);
        _proxy->sort(0);
    }

    QAbstractItemModel * SortPicker::model() const {
        return _proxy->sourceModel();
    }

    void SortPicker::onComboBoxIndexChanged(int) {
        emit currentIndexChanged(currentIndex());
    }

    void SortPicker::setCurrentIndex(int index) {
        _ui->comboBox->setCurrentIndex(index);
    }

    int SortPicker::currentIndex() const {
        int index = _ui->comboBox->currentIndex();
        const QModelIndex mapped = _proxy->mapToSource(_proxy->index(index, 0));
        return mapped.row();
    }
}
