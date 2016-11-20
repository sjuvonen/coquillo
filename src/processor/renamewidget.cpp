
#include <QDebug>
#include <QFileInfo>
#include <QItemSelectionModel>
#include <QSettings>
#include <QTimer>

#include "tags/tagdataroles.hpp"
#include "paths.hpp"
#include "patterns.hpp"
#include "renamewidget.hpp"
#include "ui_renamewidget.h"

namespace Coquillo {
    namespace Processor {
        RenameWidget::RenameWidget(QWidget * parent)
        : PatternWidget(parent) {
            _ui = new Ui::RenameWidget;
            _ui->setupUi(this);
            setupUi();
        }

        RenameWidget::~RenameWidget() {
            delete _ui;
        }

        void RenameWidget::applyPattern(const QString & pattern) {
            qDebug() << "apply pattern" << pattern;

            const Patterns patterns;
            const Paths paths;
            QList<QPersistentModelIndex> indices;

            /**
             * If the model has automatic sorting enabled, rows might change order in between renames.
             * Use persistent indices to work around this problem.
             */
            foreach (const QModelIndex idx, selectionModel()->selectedRows()) {
                indices << QPersistentModelIndex(idx);
            }

            foreach (const QPersistentModelIndex idx, indices) {
                const QVariantHash values = sourceValues(idx);
                const QString suffix = QFileInfo(values["filename"].toString()).suffix();
                const QString compiled = patterns.compile(this->pattern(), values);
                const QString filename = QString("%1.%2").arg(compiled, suffix);
                const QString path = paths.mergeFileNames(idx.data(Tags::FilePathRole).toString(), filename);
                model()->setData(idx, path, Tags::FilePathRole);
            }
        }

        void RenameWidget::updatePreview() {
            if (!selectionModel()) {
                return;
            }

            const Patterns patterns;
            const QVariantHash values = sourceValues(currentIndex());
            const QString suffix = QFileInfo(values["filename"].toString()).suffix();
            const QString text = patterns.compile(pattern(), values);
            const QString name = QString("%1.%2").arg(text, suffix);
            setPreview(name);
        }

        QVariantHash RenameWidget::sourceValues(const QModelIndex & idx) const {
            int pad_width = QSettings().value("NumberPadWidth").toInt();
            QVariantHash values = idx.data(Tags::ValuesMapRole).toHash();
            values["padded_disc"] = values["disc"].toString().rightJustified(pad_width, '0');
            values["padded_number"] = values["number"].toString().rightJustified(pad_width, '0');
            values["padded_year"] = values["year"].toString().rightJustified(pad_width, '0');
            return values;
        }
    }
}
