#ifndef COQUILLO_PROCESSOR_PARSERWIDGET_H
#define COQUILLO_PROCESSOR_PARSERWIDGET_H

#include "patternwidget.h"

namespace Ui {
    class ParserWidget;
}

namespace Coquillo {
    namespace Processor {
        class ParserWidget : public PatternWidget {
            Q_OBJECT

            public:
                ParserWidget(QWidget * parent = 0);
                ~ParserWidget();

            protected slots:
                void applyPattern(const QString & pattern);
                void updatePreview();

            private:
                Ui::ParserWidget * _ui;
        };
    }
}

#endif
