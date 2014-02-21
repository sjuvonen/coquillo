#ifndef COQUILLO_PROCESSOR_RENAMEWIDGET_H
#define COQUILLO_PROCESSOR_RENAMEWIDGET_H

#include <QWidget>

class QAbstractItemModel;
class QItemSelectionModel;

namespace Ui {
    class RenameWidget;
}

namespace Coquillo {
    class HistoryModel;

    namespace Processor {
        class RenameWidget : public QWidget {
            Q_OBJECT

            public:
                RenameWidget(QWidget * parent = 0);
                ~RenameWidget();
                void setHistoryModel(QAbstractItemModel * history);
                void setModel(QAbstractItemModel * model);
                void setSelectionModel(QItemSelectionModel * model);

                inline QAbstractItemModel * model() const { return _model; }
                inline QAbstractItemModel * historyModel() const { return _history; }
                inline QItemSelectionModel * selectionModel() const { return _selectionModel; }

            signals:
                void patternAccepted(const QString & pattern);

            private slots:
                void apply();
                void applyPattern(const QString & pattern);

            private:
                Ui::RenameWidget * _ui;
                QAbstractItemModel * _history;
                QAbstractItemModel * _model;
                QItemSelectionModel * _selectionModel;
        };
    }
}

#endif
