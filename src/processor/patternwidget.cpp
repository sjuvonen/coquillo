
#include <QComboBox>
#include <QDebug>
#include <QLabel>
#include <QPushButton>

#include <metadata/metadatamodel.h>
#include "patterns.h"
#include "patternwidget.h"

namespace Coquillo {
    namespace Processor {
        PatternWidget::PatternWidget(QWidget * parent)
        : BaseWidget(parent), _history(0) {
            // Initialization is done in setupUi() that's called by
            // the inheriting classes.
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

            connect(apply, SIGNAL(clicked()), SLOT(applyCurrentPattern()));
            connect(pattern, SIGNAL(activated(QString)), SIGNAL(patternAccepted(QString)));
        }

        void PatternWidget::updatePreview() {
            if (!selectionModel()) {
                return;
            }

            QLabel * preview = findChild<QLabel*>("preview");

            if (preview) {
                Patterns patterns;
                const QVariantHash values = currentIndex().data(MetaDataModel::NamedRowDataRole).toHash();
                const QString text = patterns.compile(pattern(), values);
                preview->setText(text);
            }
        }

        void PatternWidget::applyCurrentPattern() {
            if (_history) {
                QComboBox * widget = findChild<QComboBox*>("pattern");
                if (widget->findText(pattern()) == -1) {
                    widget->addItem(pattern());
                    _history->submit();
                }
            }

            emit patternAccepted(pattern());
        }
    }
}
