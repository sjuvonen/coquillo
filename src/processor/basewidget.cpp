
#include <QAbstractItemModel>
#include <QItemSelectionModel>
#include <QDebug>

#include "basewidget.hpp"

namespace Coquillo {
    namespace Processor {
        BaseWidget::BaseWidget(QWidget * parent)
        : QWidget(parent) {

        }

        void BaseWidget::setModel(QAbstractItemModel * model) {
            _model = QPointer<QAbstractItemModel>(model);
            setSelectionModel(new QItemSelectionModel(model));
        }

        QAbstractItemModel * BaseWidget::model() const {
            return _model.data();
        }

        void BaseWidget::setSelectionModel(QItemSelectionModel * model) {
            if (_selectionModel) {
                delete _selectionModel.data();
            }

            _selectionModel = QPointer<QItemSelectionModel>(model);
            connect(model, SIGNAL(currentChanged(QModelIndex, QModelIndex)), SLOT(updatePreview()));
        }

        QItemSelectionModel * BaseWidget::selectionModel() const {
            return _selectionModel.data();
        }

        QModelIndex BaseWidget::currentIndex() const {
            return selectionModel()->currentIndex();
        }
    }
}
