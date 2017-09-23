#include <QAbstractItemModel>
#include <QThreadPool>

#include "searcher/fetchresultjob.hpp"
#include "searcher/searchjob.hpp"
#include "albumdetailsmodel.hpp"
#include "selectionfiltermodel.hpp"
#include "tagsearchdialog.hpp"
#include "varianthashmodel.hpp"
#include "ui_tagsearchdialog.h"

#include <QDebug>
#include <QTimer>

namespace Coquillo {
    namespace WebTags {
        TagSearchDialog::TagSearchDialog(QWidget * parent)
        : QDialog(parent) {
            _ui = new Ui::TagSearchDialog;
            _ui->setupUi(this);

            _ui->tabs->removeTab(1);

            _ui->progressBar->hide();
            _ui->textAlbum->setFocus();

            _results = new VariantHashModel(this);
            _results->setKeys({"title", "artist", "disc", "id"});
            _results->setHeaderLabels({tr("Album"), tr("Artist"), tr("D#"), tr("ID")});

            _ui->listSearchResults->setModel(_results);
            _ui->listSearchResults->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            _ui->listSearchResults->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
            _ui->listSearchResults->horizontalHeader()->resizeSection(1, 150);
            _ui->listSearchResults->horizontalHeader()->resizeSection(2, 50);
            _ui->listSearchResults->horizontalHeader()->hideSection(3);

            _details = new AlbumDetailsModel(this);

            _ui->listResultView->setModel(_details);
            _ui->listResultView->setRootIndex(QModelIndex());
            _ui->listResultView->horizontalHeader()->hideSection(2);
            _ui->listResultView->horizontalHeader()->hideSection(3);
            _ui->listResultView->horizontalHeader()->hideSection(4);
            _ui->listResultView->horizontalHeader()->hideSection(5);
            _ui->listResultView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
            // _ui->listResultView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

            connect(_ui->listSearchResults->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), SLOT(showSearchResult(QModelIndex)));

            _ui->listResultViewAlt->setModel(_details);
            _ui->listResultViewAlt->setRootIndex(QModelIndex());
            _ui->listResultViewAlt->horizontalHeader()->hideSection(2);
            _ui->listResultViewAlt->horizontalHeader()->hideSection(3);
            _ui->listResultViewAlt->horizontalHeader()->hideSection(4);
            _ui->listResultViewAlt->horizontalHeader()->hideSection(5);
            _ui->listResultViewAlt->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

            _ui->listSelectedFiles->setModel(new SelectionFilterModel);
        }

        TagSearchDialog::~TagSearchDialog() {
            delete _ui;
        }

        QList<QVariantHash> TagSearchDialog::resultData() const {
            return QList<QVariantHash>();
        }

        void TagSearchDialog::setModel(QAbstractItemModel * model) {
            _model = model;
            qobject_cast<SelectionFilterModel*>(_ui->listSelectedFiles->model())->setSourceModel(model);
        }

        QAbstractItemModel * TagSearchDialog::model() const {
            return _model;
        }

        void TagSearchDialog::setSelectedRows(const QList<QModelIndex> & rows) {
            qobject_cast<SelectionFilterModel*>(_ui->listSelectedFiles->model())->setSelection(rows);
        }

        void TagSearchDialog::on_buttonSearch_clicked() {
            const QString album = _ui->textAlbum->text();
            const QString artist = _ui->textArtist->text();

            search({{"artist", artist}, {"album", album}});
        }

        void TagSearchDialog::search(const QVariantMap & values) {
            auto * job = new Searcher::SearchJob(values);
            auto * model = _ui->listSearchResults->model();

            model->removeRows(0, model->rowCount());

            _ui->progressBar->show();

            connect(job, &Searcher::SearchJob::finished, [=] {
                _ui->progressBar->hide();
                _results->setData(job->result());
            });

            QThreadPool::globalInstance()->start(job);
        }

        void TagSearchDialog::showSearchResult(const QModelIndex & idx) {
            const QString disc_id = idx.sibling(idx.row(), 3).data().toString();
            int disc_nr = idx.sibling(idx.row(), 2).data().toInt();
            const QModelIndex index = _details->findResult(disc_id, disc_nr);

            if (index.isValid()) {
                _ui->listResultView->setRootIndex(index);
            } else {
                auto * job = new Searcher::FetchResultJob(disc_id, disc_nr);

                _ui->progressBar->show();

                connect(job, &Searcher::FetchResultJob::finished, this, [=] {
                    _ui->progressBar->hide();

                    auto index = _details->appendResult(job->result());
                    _ui->listResultView->setRootIndex(index);
                    _ui->listResultViewAlt->setRootIndex(index);
                    job->deleteLater();
                });

                QThreadPool::globalInstance()->start(job);
            }
        }
    }
}
