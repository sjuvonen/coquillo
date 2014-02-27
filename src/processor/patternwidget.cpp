
#include <QComboBox>
#include <QLabel>

#include <metadata/metadatamodel.h>
#include "historymodel.h"
#include "patterns.h"
#include "patternwidget.h"

namespace Coquillo {
    namespace Processor {
        PatternWidget::PatternWidget(QWidget * parent)
        : BaseWidget(parent), _history(0) {

        }

        void PatternWidget::setHistoryModel(QAbstractItemModel * history) {
            QComboBox * pattern = findChild<QComboBox*>("pattern");

            if (pattern) {
                if (_history) {
                    disconnect(pattern, SIGNAL(activated(QString)), _history, SLOT(submit()));
                }

                _history = history;
                pattern->setModel(history);
                connect(pattern, SIGNAL(activated(QString)), _history, SLOT(submit()));
            }
        }

        QString PatternWidget::pattern() const {
            QComboBox * widget = findChild<QComboBox*>("pattern");
            return widget ? widget->currentText() : QString();
        }

        void PatternWidget::updatePreview() {
            if (!selectionModel()) {
                return;
            }

            QLabel * preview = findChild<QLabel*>("preview");

            Patterns patterns;
            const QVariantHash values = currentIndex().data(MetaDataModel::NamedRowDataRole).toHash();
            const QString text = patterns.compile(pattern(), values);
            preview->setText(text);
        }
    }
}
