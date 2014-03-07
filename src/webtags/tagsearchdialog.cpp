
#include <QAbstractItemModel>

#include <metadata/metadatamodel.h>
#include "filterproxymodel.h"
#include "tagsearchdialog.h"
#include "ui_tagsearchdialog.h"

namespace Coquillo {
    namespace WebTags {
        TagSearchDialog::TagSearchDialog(QWidget * parent)
        : QDialog(parent) {
            _ui = new Ui::TagSearchDialog;
            _ui->setupUi(this);
            _ui->source->setModel(new FilterProxyModel(this));

            FilterProxyModel * selected = new FilterProxyModel(this);
            selected->setFilterMode(FilterProxyModel::ShowFiltered);
            _ui->selected->setModel(selected);
        }

        TagSearchDialog::~TagSearchDialog() {
            delete _ui;
        }

        void TagSearchDialog::setModel(QAbstractItemModel * model) {
            qobject_cast<FilterProxyModel*>(_ui->source->model())->setSourceModel(model);
            qobject_cast<FilterProxyModel*>(_ui->selected->model())->setSourceModel(model);
            _ui->source->setModelColumn(1);
            _model = model;

            for (int i = 0; i < _ui->selected->horizontalHeader()->count(); i++) {
                if (i != _ui->source->modelColumn()) {
                    _ui->selected->horizontalHeader()->hideSection(i);
                }
            }
        }

        QAbstractItemModel * TagSearchDialog::model() const {
            return _model.data();
        }

        void TagSearchDialog::moveCurrentDown() {

        }

        void TagSearchDialog::moveCurrentUp() {

        }

        void TagSearchDialog::selectCurrent() {
            QModelIndexList indices = _ui->source->selectionModel()->selectedRows();
            QStringList filters;

            foreach (const QModelIndex idx, indices) {
                const QString path = idx.data(Coquillo::MetaDataModel::FilePathRole).toString();
                filters << path;
            }

            selectPaths(filters);
        }

        void TagSearchDialog::unselectCurrent() {
            QModelIndexList indices = _ui->selected->selectionModel()->selectedRows();
            QStringList filters;

            foreach (const QModelIndex idx, indices) {
                const QString path = idx.data(Coquillo::MetaDataModel::FilePathRole).toString();
                filters << path;
            }

            unselectPaths(filters);
        }

        void TagSearchDialog::selectPaths(const QStringList & paths) {
            qobject_cast<FilterProxyModel*>(_ui->source->model())->addFilters(paths);
            qobject_cast<FilterProxyModel*>(_ui->selected->model())->addFilters(paths);
        }

        void TagSearchDialog::unselectPaths(const QStringList & paths) {
            foreach (const QString path, paths) {
                qobject_cast<FilterProxyModel*>(_ui->source->model())->removeFilter(path);
                qobject_cast<FilterProxyModel*>(_ui->selected->model())->removeFilter(path);
            }
        }
    }
}
