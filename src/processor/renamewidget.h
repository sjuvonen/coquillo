#ifndef COQUILLO_PROCESSOR_RENAMEWIDGET_H
#define COQUILLO_PROCESSOR_RENAMEWIDGET_H

#include <QWidget>

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
                void setHistoryModel(Coquillo::HistoryModel * history);
                inline Coquillo::HistoryModel * historyModel() const { return _history; }

            signals:
                void patternAccepted(const QString & pattern);

            private slots:
                void apply();

            private:
                Ui::RenameWidget * _ui;
                Coquillo::HistoryModel * _history;
        };
    }
}

#endif
