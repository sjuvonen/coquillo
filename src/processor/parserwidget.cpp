
#include <QCompleter>
#include <QDebug>
#include <QDir>
#include <QItemSelectionModel>

#include "tags/tagdataroles.hpp"
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

            _ui->pattern->completer()->setCaseSensitivity(Qt::CaseSensitive);
        }

        ParserWidget::~ParserWidget() {
            delete _ui;
        }

        void ParserWidget::applyPattern(const QString & pattern) {
            Q_UNUSED(pattern)
            const QString path = filePathSliceForPattern(pattern);
            const QVariantMap tags = Patterns().extract(pattern, path);

            foreach (const QModelIndex & idx, selectionModel()->selectedRows()) {
                model()->setData(idx, tags, Tags::ValuesMapRole);
            }

        }

        void ParserWidget::updatePreview() {
            const QString pattern = this->pattern();
            const QString path = filePathSliceForPattern(pattern);
            const QVariantMap tags = Patterns().extract(pattern, path);
            QStringList parts;

            foreach (QString key, tags.keys()) {
                parts << QString("<b>%1</b>: %2").arg(key, tags[key].toString());
            }

            parts.sort(Qt::CaseInsensitive);
            setPreview(parts.join(", "));
        }

        QString ParserWidget::filePathSliceForPattern(const QString & pattern) const {
            const QChar separator = QDir::separator();
            QString path = currentIndex().data(Tags::FilePathRole).toString();
            path = path.section(separator, -1 - pattern.count(separator));
            path = path.left(path.length() - QFileInfo(path).suffix().length() - 1);
            return path;
        }
    }
}
