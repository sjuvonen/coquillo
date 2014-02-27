#ifndef COQUILLO_PROCESSOR_RENAMEWIDGET_H
#define COQUILLO_PROCESSOR_RENAMEWIDGET_H

#include "patternwidget.h"

class QAbstractItemModel;
class QItemSelectionModel;

namespace Ui {
    class RenameWidget;
}

namespace Coquillo {
    class HistoryModel;

    namespace Processor {
        class RenameWidget : public PatternWidget {
            Q_OBJECT

            public:
                RenameWidget(QWidget * parent = 0);
                ~RenameWidget();
//                 void setHistoryModel(QAbstractItemModel * history);
//                 void setModel(QAbstractItemModel * model);
//                 QAbstractItemModel * model() const;
//                 void setSelectionModel(QItemSelectionModel * model);
//                 QItemSelectionModel * selectionModel() const;
//                 QModelIndex currentIndex() const;
//                 QString pattern() const;

//                 inline QAbstractItemModel * historyModel() const { return _history; }

//             signals:
//                 void patternAccepted(const QString & pattern);

            protected slots:
                void apply();

            private slots:
                void applyPattern(const QString & pattern);
//                 void updatePreview();

            private:
                Ui::RenameWidget * _ui;
//                 QAbstractItemModel * _history;
//                 QPointer<QAbstractItemModel> _model;
//                 QPointer<QItemSelectionModel> _selectionModel;
        };
    }
}

#endif
