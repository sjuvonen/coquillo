
#include <QComboBox>
#include <QDebug>
#include <QItemSelectionModel>
#include <QLabel>
#include <QPushButton>

#include "patternwidget.hpp"

namespace Coquillo {
    namespace Processor {
        PatternWidget::PatternWidget(QWidget * parent)
        : BaseWidget(parent), _history(0) {
            // Initialization is done in setupUi() that's called by
            // the inheriting classes.
            connect(this, SIGNAL(patternAccepted(QString)), SLOT(applyPattern(QString)));
        }

        void PatternWidget::setSelectionModel(QItemSelectionModel * model) {
            if (model != selectionModel()) {
                if (selectionModel()) {
                    disconnect(selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)), this, SLOT(updatePreview()));
                }

                if (model) {
                    connect(model, SIGNAL(currentChanged(QModelIndex, QModelIndex)), this, SLOT(updatePreview()));
                }
            }

            BaseWidget::setSelectionModel(model);
        }

        void PatternWidget::setHistoryModel(QAbstractItemModel * history) {
            QComboBox * pattern = findChild<QComboBox*>("pattern");

            if (pattern) {
                if (_history) {
                    disconnect(pattern, SIGNAL(activated(QString)), _history, SLOT(submit()));
                }

                if (history) {
                    pattern->setModel(history);
                    connect(pattern, SIGNAL(activated(QString)), history, SLOT(submit()));
                }
            }

            _history = history;
        }

        QString PatternWidget::pattern() const {
            QComboBox * widget = findChild<QComboBox*>("pattern");
            return widget ? widget->currentText() : QString();
        }

        void PatternWidget::apply() {
            emit patternAccepted(pattern());
        }

        void PatternWidget::setupUi() {
            QPushButton * apply = findChild<QPushButton*>("apply");
            QComboBox * pattern = findChild<QComboBox*>("pattern");

            connect(pattern, SIGNAL(editTextChanged(QString)), SLOT(updatePreview()));
            connect(apply, SIGNAL(clicked()), SLOT(applyCurrentPattern()));
        }

        void PatternWidget::setPreview(const QString & text) {
            QLabel * preview = findChild<QLabel*>("preview");

            if (preview) {
                preview->setText(text);
            }
        }

        void PatternWidget::applyCurrentPattern() {
            if (_history) {
                const QString pattern = this->pattern();
                QComboBox * widget = findChild<QComboBox*>("pattern");
                if (pattern.length() && widget->findText(pattern) == -1) {
                    widget->addItem(pattern);
                    _history->submit();
                }
            }

            emit patternAccepted(pattern());
        }
    }
}
