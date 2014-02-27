#ifndef COQUILLO_PROCESSOR_BASEWIDGET_H
#define COQUILLO_PROCESSOR_BASEWIDGET_H

#include <QPointer>
#include <QWidget>

class QAbstractItemModel;
class QItemSelectionModel;

namespace Coquillo {
    namespace Processor {
        class BaseWidget : public QWidget {
            Q_OBJECT

            public:
                BaseWidget(QWidget * parent = 0);
                virtual ~BaseWidget() { }

                QModelIndex currentIndex() const;

                QAbstractItemModel * model() const;
                void setModel(QAbstractItemModel * model);

                QItemSelectionModel * selectionModel() const;
                void setSelectionModel(QItemSelectionModel * model);

            protected slots:
                virtual void apply() = 0;

            private:
                QPointer<QAbstractItemModel> _model;
                QPointer<QItemSelectionModel> _selectionModel;

        };
    }
}

#endif
