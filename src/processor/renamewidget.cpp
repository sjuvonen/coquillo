
#include <QAbstractItemModel>
#include <QItemSelectionModel>
#include <QDebug>

#include "historymodel.h"
#include "renamewidget.h"
#include "ui_renamewidget.h"

namespace Coquillo {
    namespace Processor {
        RenameWidget::RenameWidget(QWidget * parent)
        : QWidget(parent), _history(0), _model(0), _selectionModel(0) {
            _ui = new Ui::RenameWidget;
            _ui->setupUi(this);

            connect(_ui->apply, SIGNAL(clicked()), SLOT(apply()));
            connect(_ui->pattern, SIGNAL(activated(QString)), SLOT(apply()));
        }

        RenameWidget::~RenameWidget() {
            delete _ui;
        }

        void RenameWidget::setHistoryModel(QAbstractItemModel * history) {
            if (_history) {
                disconnect(_ui->pattern, SIGNAL(activated(QString)), _history, SLOT(submit()));
            }
            _history = history;
            _ui->pattern->setModel(history);
            _history->connect(_ui->pattern, SIGNAL(activated(QString)), _history, SLOT(submit()));
        }

        void RenameWidget::setModel(QAbstractItemModel * model) {
            _model = model;
        }

        void RenameWidget::setSelectionModel(QItemSelectionModel * model) {
            _selectionModel = model;
        }

        void RenameWidget::apply() {
            applyPattern(_ui->pattern->currentText());
        }

        void RenameWidget::applyPattern(const QString & pattern) {
            qDebug() << "apply pattern" << pattern;

            if (!_ui->pattern->findText(pattern)) {
                _ui->pattern->addItem(pattern);
                _history->submit();
            }
        }
    }
}
