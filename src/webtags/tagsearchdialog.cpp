
#include <QDebug>
#include <QStandardItemModel>

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

            QStandardItemModel * search_results = new QStandardItemModel(this);
            search_results->setHorizontalHeaderLabels(QStringList() << tr("Title") << tr("Artist") << tr("D#") << tr("ID"));
            _ui->listSearchResults->setModel(search_results);

            addSearcher("musicbrainz", new Searcher::MusicBrainz(this));
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
                const int role = Coquillo::MetaData::MetaDataModel::MetaDataRole;
                items << model->index(i, 0).data(role).value<Coquillo::MetaData::MetaData>();
            }

            _ui->selected->verticalHeader()->show();
        }

        QAbstractItemModel * TagSearchDialog::model() const {
            return _model.data();
        }

        void TagSearchDialog::addSearcher(const QString & id, Searcher::AbstractSearcher * searcher) {
            _searchers[id] = searcher;

            connect(searcher, SIGNAL(searchFinished(QList<QVariantMap>)),
                SLOT(showResults(QList<QVariantMap>)));
        }

        void TagSearchDialog::showResults(const QList<QVariantMap> & results) {
            qDebug() << "got results:" << results.count();
            QStandardItemModel * model = qobject_cast<QStandardItemModel*>(_ui->listSearchResults->model());

            foreach (const QVariantMap row, results) {
                model->appendRow(QList<QStandardItem*>()
                    << new QStandardItem(row["title"].toString())
                    << new QStandardItem(row["artist"].toString())
                    << new QStandardItem(row["disc"].toString())
                    << new QStandardItem(row["id"].toString()));
            }
        }

        void TagSearchDialog::search(const QVariantMap & data) {
            qDebug() << "search with sources: " << _searchers.count();
            _ui->mainTabs->setCurrentIndex(1);
            foreach (Searcher::AbstractSearcher * s, _searchers.values()) {
                s->search(data);
            }
        }

        void TagSearchDialog::executeSearch() {
            QVariantMap data;
            const QString artist = _ui->textArtist->text();
            const QString album = _ui->textAlbum->text();

            if (artist.length()) {
                data["artist"] = _ui->textArtist->text();
            }

            if (album.length()) {
                data["album"] = _ui->textAlbum->text();
            }

            if (data.count()) {
                search(data);
            }
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
