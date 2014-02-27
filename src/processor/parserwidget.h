#ifndef COQUILLO_PROCESSOR_PARSERWIDGET_H
#define COQUILLO_PROCESSOR_PARSERWIDGET_H

#include "patternwidget.h"

namespace Coquillo {
    namespace Processor {
        class ParserWidget : public PatternWidget {
            Q_OBJECT

            public:
                ParserWidget(QWidget * parent = 0);
                ~ParserWidget();
        };
    }
}

#endif
