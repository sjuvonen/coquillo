
#include <QDebug>
#include <QDir>

#include "metadata/metadatamodel.hpp"
#include "parserwidget.hpp"
#include "patterns.hpp"
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
            const QChar separator = QDir::separator();
            const QString pattern = this->pattern();
            QString path = currentIndex().data(MetaData::MetaDataModel::FilePathRole).toString();
            path = path.section(separator, -1 - pattern.count(separator));
            path = path.left(path.length() - QFileInfo(path).suffix().length() - 1);
            const QVariantHash tags = patterns.extract(pattern, path);
            QStringList parts;

            foreach (QString key, tags.keys()) {
                parts << QString("<b>%1</b>: %2").arg(key, tags[key].toString());
            }

            parts.sort(Qt::CaseInsensitive);
            setPreview(parts.join(", "));
        }
    }
}
