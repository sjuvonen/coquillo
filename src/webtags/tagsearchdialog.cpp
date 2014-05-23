
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
            search_results->setHorizontalHeaderLabels(QStringList() << tr("Title") << tr("Artist") << tr("D#") << tr("ID") << tr("Source"));
            _ui->tableSearchResults->setModel(search_results);

            QStandardItemModel * album_preview = new QStandardItemModel(this);
            album_preview->setHorizontalHeaderLabels(QStringList() << tr("Title"));
            _ui->tableAlbumPreview->setModel(album_preview);

            connect(_ui->tableSearchResults->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)),
                SLOT(executeFetchAlbum(QModelIndex)));

            addSearcher(new Searcher::MusicBrainz(this));
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

        void TagSearchDialog::addSearcher(Searcher::AbstractSearcher * searcher) {
            _searchers[searcher->id()] = searcher;

            connect(searcher, SIGNAL(searchReady(QList<QVariantMap>, QString)),
                SLOT(showResults(QList<QVariantMap>, QString)));

            connect(searcher, SIGNAL(albumReady(QVariantMap)),
                SLOT(showAlbumInfo(QVariantMap)));
        }

        void TagSearchDialog::executeFetchAlbum(const QModelIndex & idx) {
            qDebug() << "Fetch album info for" << idx.row();
            const QString id = idx.sibling(idx.row(), 3).data().toString();
            const QString source = idx.sibling(idx.row(), 4).data().toString();
            _searchers[source]->albumInfo(id);
        }

        void TagSearchDialog::showAlbumInfo(const QVariantMap & album) {
            qDebug() << "Show album info";

            QStandardItemModel * model = qobject_cast<QStandardItemModel*>(_ui->tableAlbumPreview->model());
            model->removeRows(0, model->rowCount());
            const QList<QVariantMap> tracks = album["tracks"].value< QList<QVariantMap> >();

            foreach (const QVariantMap row, tracks) {
                model->appendRow(QList<QStandardItem*>()
                    << new QStandardItem(row["title"].toString())
                    << new QStandardItem(row["artist"].toString()));
            }
        }

        void TagSearchDialog::showResults(const QList<QVariantMap> & results, const QString & source) {
            qDebug() << "got results:" << results.count();
            QStandardItemModel * model = qobject_cast<QStandardItemModel*>(_ui->tableSearchResults->model());

            foreach (const QVariantMap row, results) {
                model->appendRow(QList<QStandardItem*>()
                    << new QStandardItem(row["title"].toString())
                    << new QStandardItem(row["artist"].toString())
                    << new QStandardItem(row["disc"].toString())
                    << new QStandardItem(row["id"].toString())
                    << new QStandardItem(source));
            }
        }

        void TagSearchDialog::search(const QVariantMap & data) {
            qDebug() << "search with sources: " << _searchers.count();
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

            int total = _ui->tableSearchResults->model()->rowCount();
            _ui->tableSearchResults->model()->removeRows(0, total);

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
