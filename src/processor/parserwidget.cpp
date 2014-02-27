
#include <metadata/metadatamodel.h>
#include "parserwidget.h"
#include "patterns.h"
#include "ui_parserwidget.h"

namespace Coquillo {
    namespace Processor {
        ParserWidget::ParserWidget(QWidget * parent)
        : PatternWidget(parent) {
            _ui = new Ui::ParserWidget;
            _ui->setupUi(this);
            setupUi();
        }

        ParserWidget::~ParserWidget() {
            delete _ui;
        }

        void ParserWidget::applyPattern(const QString & pattern) {
            Q_UNUSED(pattern)
        }

        void ParserWidget::updatePreview() {
            Patterns patterns;
            const QString path = currentIndex().data(MetaDataModel::FilePathRole).toString();
            const QVariantHash tags = patterns.extract(pattern(), path);
            QStringList parts;

            foreach (QString key, tags.keys()) {
                parts << QString("%1: %2").arg(key, tags[key].toString());
            }

            setPreview(parts.join(", "));
        }
    }
}
