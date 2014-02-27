#ifndef COQUILLO_PROCESSOR_PATTERNWIDGET_H
#define COQUILLO_PROCESSOR_PATTERNWIDGET_H

#include "basewidget.h"

namespace Ui {
    class PatternWidget;
}

namespace Coquillo {
    namespace Processor {
        class PatternWidget : public BaseWidget {
            Q_OBJECT

            public:
                PatternWidget(QWidget * parent = 0);
                virtual ~PatternWidget() { }
                QString pattern() const;
                virtual void setSelectionModel(QItemSelectionModel * model);
                virtual void setHistoryModel(QAbstractItemModel * history);
                inline QAbstractItemModel * historyModel() const { return _history; }

            signals:
                void patternAccepted(const QString & pattern);

            protected slots:
                void apply();
                void setupUi();
                void setPreview(const QString & text);
                virtual void updatePreview() = 0;
                virtual void applyPattern(const QString &) = 0;

            private slots:
                void applyCurrentPattern();

            private:
                QAbstractItemModel * _history;

        };
    }
}

#endif
