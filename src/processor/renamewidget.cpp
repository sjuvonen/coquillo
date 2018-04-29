
#include <QCompleter>
#include <QDebug>
#include <QFileInfo>
#include <QItemSelectionModel>
#include <QSettings>
#include <QTimer>

#include "tags/tagdataroles.hpp"
#include "utils/filepaths.hpp"
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

            _ui->pattern->completer()->setCaseSensitivity(Qt::CaseSensitive);
        }

        RenameWidget::~RenameWidget() {
            delete _ui;
        }

        void RenameWidget::applyPattern(const QString & pattern) {
            const Patterns patterns;
            QList<QPersistentModelIndex> indices;

            /**
             * If the model has automatic sorting enabled, rows might change order in between renames.
             * Use persistent indices to work around this problem.
             */
            foreach (const QModelIndex idx, selectionModel()->selectedRows()) {
                indices << QPersistentModelIndex(idx);
            }

            foreach (const QPersistentModelIndex idx, indices) {
                const QVariantMap values = sourceValues(idx);
                const QString suffix = QFileInfo(values["filename"].toString()).suffix();
                const QString compiled = patterns.compile(this->pattern(), values);
                const QString filename = QString("%1.%2").arg(compiled, suffix);
                const QString path = Utils::FilePaths::mergeFileNames(idx.data(Tags::FilePathRole).toString(), filename);
                model()->setData(idx, path, Tags::FilePathRole);
            }
        }

        void RenameWidget::updatePreview() {
            if (!selectionModel()) {
                return;
            }

            const QVariantMap values = sourceValues(currentIndex());
            const QString suffix = QFileInfo(values["filename"].toString()).suffix();
            const QString text = Patterns().compile(pattern(), values);
            const QString name = QString("%1.%2").arg(text, suffix);

            if (QSettings().value("Filter/SafeFilenames").toBool()) {
                setPreview(Utils::FilePaths::safeFilePath(name));
            } else {
                setPreview(name);
            }
        }

        QVariantMap RenameWidget::sourceValues(const QModelIndex & idx) const {
            int pad_width = QSettings().value("NumberPadWidth").toInt();
            QVariantMap values = idx.data(Tags::ValuesMapRole).toMap();
            values["padded_disc"] = values["disc"].toString().rightJustified(pad_width, '0');
            values["padded_number"] = values["number"].toString().rightJustified(pad_width, '0');
            values["padded_year"] = values["year"].toString().rightJustified(pad_width, '0');
            return values;
        }
    }
}
