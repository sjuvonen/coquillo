
#include <QAbstractItemModel>
#include <QDebug>

#include <metadata/metadatamodel.h>
#include <searcher/musicbrainz.h>
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
            static_cast<FilterProxyModel*>(_ui->source->model())->setSourceModel(model);
            static_cast<FilterProxyModel*>(_ui->selected->model())->setSourceModel(model);
            _ui->source->setModelColumn(1);
            _model = model;

            for (int i = 0; i < _ui->selected->horizontalHeader()->count(); i++) {
                if (i != _ui->source->modelColumn()) {
                    _ui->selected->horizontalHeader()->hideSection(i);
                }
            }

            QList<Coquillo::MetaData::MetaData> items;

            for (int i = 0; i < model->rowCount(); i++) {
                items << model->index(i, 0).data(Coquillo::MetaData::MetaDataModel::MetaDataRole).value<Coquillo::MetaData::MetaData>();
            }

//             Coquillo::Searcher::MusicBrainz searcher;
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
                const QString path = idx.data(Coquillo::MetaData::MetaDataModel::FilePathRole).toString();
                filters << path;
            }

            selectPaths(filters);
        }

        void TagSearchDialog::unselectCurrent() {
            QModelIndexList indices = _ui->selected->selectionModel()->selectedRows();
            QStringList filters;

            foreach (const QModelIndex idx, indices) {
                const QString path = idx.data(Coquillo::MetaData::MetaDataModel::FilePathRole).toString();
                filters << path;
            }

            unselectPaths(filters);
        }

        void TagSearchDialog::selectPaths(const QStringList & paths) {
            static_cast<FilterProxyModel*>(_ui->source->model())->addFilters(paths);
            static_cast<FilterProxyModel*>(_ui->selected->model())->addFilters(paths);
        }

        void TagSearchDialog::unselectPaths(const QStringList & paths) {
            foreach (const QString path, paths) {
                static_cast<FilterProxyModel*>(_ui->source->model())->removeFilter(path);
                static_cast<FilterProxyModel*>(_ui->selected->model())->removeFilter(path);
            }
        }
    }
}
