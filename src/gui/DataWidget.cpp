
#include <QDebug>
#include <QItemSelection>

#include "DataWidget.h"

DataWidget::DataWidget(QWidget * parent)
: QWidget(parent), _model(0) {

	
}

DataWidget::~DataWidget() {
	
}

void DataWidget::setModel(QAbstractItemModel * model) {
	_model = model;
	_rows.clear();
}

void DataWidget::setSelection(const QItemSelection & selection) {
	_rows.clear();

	QList<int> rows;

	foreach (const QModelIndex idx, selection.indexes()) {
		if (!rows.contains(idx.row())) {
			rows << idx.row();
			_rows << QPersistentModelIndex(idx);
		}
	}
}