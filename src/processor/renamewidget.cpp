
#include <QAbstractItemModel>
#include <QItemSelectionModel>
#include <QDebug>

#include <metadata/metadatamodel.h>
#include "historymodel.h"
#include "patterns.h"
#include "renamewidget.h"
#include "ui_renamewidget.h"

namespace Coquillo {
    namespace Processor {
        RenameWidget::RenameWidget(QWidget * parent)
        : PatternWidget(parent) {
            _ui = new Ui::RenameWidget;
            _ui->setupUi(this);

            connect(_ui->apply, SIGNAL(clicked()), SLOT(apply()));
            connect(_ui->pattern, SIGNAL(activated(QString)), SLOT(apply()));
        }

        RenameWidget::~RenameWidget() {
            delete _ui;
        }

        void RenameWidget::apply() {
            applyPattern(pattern());
        }

        void RenameWidget::applyPattern(const QString & pattern) {
            qDebug() << "apply pattern" << pattern;

//             if (!_ui->pattern->findText(pattern)) {
//                 _ui->pattern->addItem(pattern);
//                 _history->submit();
//             }

            Patterns patterns;

            foreach (const QModelIndex idx, selectionModel()->selectedRows()) {
                const QVariantHash values = currentIndex().data(MetaDataModel::NamedRowDataRole).toHash();
                const QString path = patterns.compile(this->pattern(), values);
                model()->setData(idx, path, MetaDataModel::FileNameRole);
            }
        }
    }
}
