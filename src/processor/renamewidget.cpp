
#include <QDebug>

#include "historymodel.h"
#include "renamewidget.h"
#include "ui_renamewidget.h"

namespace Coquillo {
    namespace Processor {
        RenameWidget::RenameWidget(QWidget * parent)
        : QWidget(parent) {
            _ui = new Ui::RenameWidget;
            _ui->setupUi(this);

            connect(_ui->apply, SIGNAL(clicked()), SLOT(apply()));
            connect(_ui->pattern, SIGNAL(activated(int)), SLOT(apply()));
        }

        RenameWidget::~RenameWidget() {
            delete _ui;
        }

        void RenameWidget::setHistoryModel(HistoryModel * history) {
            _history = history;
            _ui->pattern->setModel(history);
        }

        void RenameWidget::apply() {
            _history->save();
            emit patternAccepted(_ui->pattern->currentText());
        }
    }
}
