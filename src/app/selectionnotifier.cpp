
#include "selectionnotifier.h"
#include <algorithm>
#include <qabstractitemmodel.h>
#include <qdebug.h>
#include <qlogging.h>

namespace Coquillo {
SelectionNotifier::SelectionNotifier(QObject *parent) : QObject(parent), _current(-1) {}

QAbstractItemModel *SelectionNotifier::model() { return _model; }

void SelectionNotifier::setModel(QAbstractItemModel *model) { _model = model; }

void SelectionNotifier::setSelectionModel(QItemSelectionModel *selectionModel) {
    _selectionModel = selectionModel;

    connect(selectionModel, &QItemSelectionModel::selectionChanged,
            [this](const QItemSelection &selected) {
                QList<int> rows;

                for (const auto idx : _selectionModel->selectedRows()) {
                    rows << idx.row();
                }

                setSelection(rows);
            });
}

void SelectionNotifier::setSelection(const QList<int> &selection) {
    _selection = selection;

    std::sort(_selection.begin(), _selection.end());

    emit selectionChanged();

    if (!_selection.contains(_current)) {
        setCurrent(selection.size() ? selection[0] : -1);
    }
}

void SelectionNotifier::setCurrent(int row) {
    const int old = _current;
    const int size = _selection.size();

    if (_selection.empty()) {
        _current = -1;
    } else if (!_selection.contains(row)) {
        _current = _selection[0];
    } else {
        _current = row;
    }

    if (_current != old) {
        emit currentChanged(_current);
    }
}

void SelectionNotifier::next() {
    if (!_selection.empty()) {
        int i = _selection.indexOf(_current);

        if (i == _selection.size() - 1) {
            setCurrent(0);
        } else {
            setCurrent(i + 1);
        }
    }
}

void SelectionNotifier::previous() {
    if (!_selection.empty()) {
        int i = _selection.indexOf(_current);

        if (i == 0) {
            setCurrent(0);
        } else {
            setCurrent(i + 1);
        }
    }
}
} // namespace Coquillo