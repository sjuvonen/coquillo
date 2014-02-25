
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
        : QWidget(parent), _history(0), _selectionModel(0) {
            _ui = new Ui::RenameWidget;
            _ui->setupUi(this);

            connect(_ui->apply, SIGNAL(clicked()), SLOT(apply()));
            connect(_ui->pattern, SIGNAL(activated(QString)), SLOT(apply()));
        }

        RenameWidget::~RenameWidget() {
            delete _ui;
        }

        void RenameWidget::setHistoryModel(QAbstractItemModel * history) {
            if (_history) {
                disconnect(_ui->pattern, SIGNAL(activated(QString)), _history, SLOT(submit()));
            }
            _history = history;
            _ui->pattern->setModel(history);
            connect(_ui->pattern, SIGNAL(activated(QString)), _history, SLOT(submit()));
        }

        void RenameWidget::setModel(QAbstractItemModel * model) {
            if (_selectionModel) {
                delete _selectionModel;
            }

            _model = QPointer<QAbstractItemModel>(model);
            _selectionModel = new QItemSelectionModel(model);
        }

        QAbstractItemModel * RenameWidget::model() const {
            return _model.data();
        }

        void RenameWidget::setSelectionModel(QItemSelectionModel * model) {
            _selectionModel = model;
            connect(model, SIGNAL(currentChanged(QModelIndex, QModelIndex)), SLOT(updatePreview()));
        }

        QItemSelectionModel * RenameWidget::selectionModel() const {
            return _selectionModel.data();
        }

        QModelIndex RenameWidget::currentIndex() const {
            return selectionModel()->currentIndex();
        }

        QString RenameWidget::pattern() const {
            return _ui->pattern->currentText();
        }

        void RenameWidget::apply() {
            applyPattern(pattern());
        }

        void RenameWidget::applyPattern(const QString & pattern) {
            qDebug() << "apply pattern" << pattern;

            if (!_ui->pattern->findText(pattern)) {
                _ui->pattern->addItem(pattern);
                _history->submit();
            }

            Patterns patterns;

            foreach (const QModelIndex idx, selectionModel()->selectedRows()) {
                const QVariantHash values = currentIndex().data(MetaDataModel::NamedRowDataRole).toHash();
                const QString path = patterns.compile(this->pattern(), values);
                model()->setData(idx, path, MetaDataModel::FileNameRole);
            }
        }

        void RenameWidget::updatePreview() {
            if (!selectionModel()) {
                return;
            }

            Patterns patterns;
            const QVariantHash values = currentIndex().data(MetaDataModel::NamedRowDataRole).toHash();
            const QString text = patterns.compile(pattern(), values);
            _ui->preview->setText(text);
        }
    }
}
