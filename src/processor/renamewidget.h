#ifndef COQUILLO_PROCESSOR_RENAMEWIDGET_H
#define COQUILLO_PROCESSOR_RENAMEWIDGET_H

#include <QPointer>
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
                QAbstractItemModel * model() const;
                void setSelectionModel(QItemSelectionModel * model);
                QItemSelectionModel * selectionModel() const;
                QModelIndex currentIndex() const;
                QString pattern() const;

                inline QAbstractItemModel * historyModel() const { return _history; }

            signals:
                void patternAccepted(const QString & pattern);

            private slots:
                void apply();
                void applyPattern(const QString & pattern);
                void updatePreview();

            private:
                Ui::RenameWidget * _ui;
                QAbstractItemModel * _history;
                QPointer<QAbstractItemModel> _model;
                QPointer<QItemSelectionModel> _selectionModel;
        };
    }
}

#endif
