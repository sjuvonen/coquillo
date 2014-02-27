#ifndef COQUILLO_PROCESSOR_PATTERNWIDGET_H
#define COQUILLO_PROCESSOR_PATTERNWIDGET_H

#include "basewidget.h"

namespace Coquillo {
    namespace Processor {
        class PatternWidget : public BaseWidget {
            Q_OBJECT

            public:
                PatternWidget(QWidget * parent = 0);
                virtual ~PatternWidget() { }

                QString pattern() const;

                void setHistoryModel(QAbstractItemModel * history);
                inline QAbstractItemModel * historyModel() const { return _history; }

            signals:
                void patternAccepted(const QString & pattern);

            protected slots:
                void updatePreview();

            private:
                QAbstractItemModel * _history;

        };
    }
}

#endif
