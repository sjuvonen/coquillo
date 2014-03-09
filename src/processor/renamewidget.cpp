
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

            /*
             * NOTE: This doesn't work when the model is QSortFilterProxyModel
             * with dynamicSortFilter enabled, because the items might change
             * rows after the path is renamed and then the list of indices will
             * refer to wrong items.
             */
            foreach (const QModelIndex idx, selectionModel()->selectedRows()) {
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
