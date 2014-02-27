#ifndef COQUILLO_PROCESSOR_RENAMEWIDGET_H
#define COQUILLO_PROCESSOR_RENAMEWIDGET_H

#include "patternwidget.h"

namespace Ui {
    class RenameWidget;
}

namespace Coquillo {
    namespace Processor {
        class RenameWidget : public PatternWidget {
            Q_OBJECT

            public:
                RenameWidget(QWidget * parent = 0);
                ~RenameWidget();

            protected slots:
                void applyPattern(const QString & pattern);

            private:
                Ui::RenameWidget * _ui;
        };
    }
}

#endif
