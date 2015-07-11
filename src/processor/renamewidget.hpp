#ifndef COQUILLO_PROCESSOR_RENAMEWIDGET_H
#define COQUILLO_PROCESSOR_RENAMEWIDGET_H

#include "patternwidget.hpp"

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
                void updatePreview();

            private:
                Ui::RenameWidget * _ui;
        };
    }
}

#endif
