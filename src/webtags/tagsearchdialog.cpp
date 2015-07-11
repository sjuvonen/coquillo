
#include <QDebug>
#include <QStandardItemModel>

#include <metadata/metadatamodel.h>
#include <searcher/musicbrainz.h>
#include "trackselectionmodel.h"
#include "tagsearchdialog.h"
#include "ui_tagsearchdialog.h"

namespace Coquillo {
    namespace WebTags {
        TagSearchDialog::TagSearchDialog(QWidget * parent)
        : QDialog(parent) {
            _ui = new Ui::TagSearchDialog;
            _ui->setupUi(this);
            _ui->listTracks->setModel(new TrackSelectionModel(this));
            _ui->listTracks->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

            QStandardItemModel * search_results = new QStandardItemModel(this);
            search_results->setHorizontalHeaderLabels(QStringList() << tr("Title") << tr("Artist") << tr("D#") << tr("ID") << tr("Source"));
            _ui->listResults->setModel(search_results);

            QStandardItemModel * album_preview = new QStandardItemModel(this);
            album_preview->setHorizontalHeaderLabels(QStringList() << tr("Title") << tr("Artist"));
            _ui->listAlbum->setModel(album_preview);
            _ui->listPreview->setModel(album_preview);

            connect(_ui->listResults->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)),
                SLOT(executeFetchAlbum(QModelIndex)));

            addSearcher(new Searcher::MusicBrainz(this));
            _ui->textAlbum->setFocus(Qt::ActiveWindowFocusReason);

            _ui->listResults->horizontalHeader()->resizeSection(0, 240);
            _ui->listResults->horizontalHeader()->resizeSection(1, 120);
            _ui->listResults->horizontalHeader()->resizeSection(2, 40);
            _ui->listResults->horizontalHeader()->resizeSection(3, 40);
            _ui->listResults->horizontalHeader()->hideSection(3);
            _ui->listResults->horizontalHeader()->hideSection(4);
            _ui->listAlbum->horizontalHeader()->resizeSection(0, 260);

            _ui->listPreview->horizontalHeader()->restoreState(_ui->listAlbum->horizontalHeader()->saveState());
            onTabChange(0);
        }

        TagSearchDialog::~TagSearchDialog() {
            delete _ui;
        }

        void TagSearchDialog::setModel(QAbstractItemModel * model) {
            qobject_cast<TrackSelectionModel*>(_ui->listTracks->model())->setSourceModel(model);
            _model = model;
        }

        QAbstractItemModel * TagSearchDialog::model() const {
            return _model;
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
            const int disc = idx.sibling(idx.row(), 2).data().toInt();
            const QString id = idx.sibling(idx.row(), 3).data().toString();
            const QString source = idx.sibling(idx.row(), 4).data().toString();
            _searchers[source]->albumInfo(id, disc-1);
        }

        void TagSearchDialog::showAlbumInfo(const QVariantMap & album) {
            qDebug() << "Show album info";

            QStandardItemModel * model = qobject_cast<QStandardItemModel*>(_ui->listAlbum->model());
            model->removeRows(0, model->rowCount());
            const QList<QVariantMap> tracks = qvariant_cast< QList<QVariantMap> >(album["tracks"]);

            foreach (const QVariantMap row, tracks) {
                model->appendRow(QList<QStandardItem*>()
                    << new QStandardItem(row["title"].toString())
                    << new QStandardItem(row["artist"].toString()));
            }
        }

        void TagSearchDialog::showResults(const QList<QVariantMap> & results, const QString & source) {
            qDebug() << "got results:" << results.count();
            QStandardItemModel * model = qobject_cast<QStandardItemModel*>(_ui->listResults->model());

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

            int total = _ui->listResults->model()->rowCount();
            _ui->listResults->model()->removeRows(0, total);

            if (data.count()) {
                search(data);
            }
        }

        void TagSearchDialog::moveCurrentDown() {
            const QModelIndexList rows = _ui->listTracks->selectionModel()->selectedRows();
            qobject_cast<TrackSelectionModel*>(_ui->listTracks->model())->shiftRows(rows, 1);
        }

        void TagSearchDialog::moveCurrentUp() {
            const QModelIndexList rows = _ui->listTracks->selectionModel()->selectedRows();
            qobject_cast<TrackSelectionModel*>(_ui->listTracks->model())->shiftRows(rows, -1);
        }

        void TagSearchDialog::onTabChange(int current) {
            _ui->dialogButtons->button(QDialogButtonBox::Apply)->setEnabled(current == 1);
        }
    }
}
