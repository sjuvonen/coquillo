
#include <QDebug>
#include <QItemSelectionModel>
#include <QTimer>

#include <metadata/metadatamodel.h>
#include "patterns.h"
#include "renamewidget.h"
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

            Patterns patterns;
            QList<QPersistentModelIndex> indices;

            foreach (const QModelIndex idx, selectionModel()->selectedRows()) {
                indices << QPersistentModelIndex(idx);
            }
            
            foreach (const QPersistentModelIndex idx, indices) {
                const QVariantHash values = idx.data(MetaData::MetaDataModel::NamedRowDataRole).toHash();
                const QString path = patterns.compile(this->pattern(), values);
                model()->setData(idx, path, MetaData::MetaDataModel::FileNameRole);
            }
        }

        void RenameWidget::updatePreview() {
            if (!selectionModel()) {
                return;
            }

            Patterns patterns;
            const QVariantHash values = currentIndex().data(MetaData::MetaDataModel::NamedRowDataRole).toHash();
            const QString text = patterns.compile(pattern(), values);
            setPreview(text);
        }
    }
}
