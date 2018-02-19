#include <QAbstractItemModel>
#include <QPersistentModelIndex>
#include <QThreadPool>

#include "searcher/fetchresultjob.hpp"
#include "searcher/searchjob.hpp"
#include "albumdetailsmodel.hpp"
#include "selectionproxymodel.hpp"
#include "tagsearchdialog.hpp"
#include "varianthashmodel.hpp"
#include "ui_tagsearchdialog.h"

#include <QDebug>
#include <QTimer>

namespace Coquillo {
    namespace WebTags {
        TagSearchDialog::TagSearchDialog(QItemSelectionModel * selection, QWidget * parent)
        : QDialog(parent) {
            _ui = new Ui::TagSearchDialog;
            _ui->setupUi(this);

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

            connect(_ui->listSearchResults->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), SLOT(showSearchResult(QModelIndex)));

            _ui->listAlbumDetails->setModel(_details);
            _ui->listAlbumDetails->setRootIndex(QModelIndex());
            _ui->listAlbumDetails->horizontalHeader()->hideSection(2);
            _ui->listAlbumDetails->horizontalHeader()->hideSection(3);
            _ui->listAlbumDetails->horizontalHeader()->hideSection(4);
            _ui->listAlbumDetails->horizontalHeader()->hideSection(5);
            _ui->listAlbumDetails->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

            _ui->listSelectedFiles->setModel(new SelectionProxyModel(selection));

            _ui->buttonBack->setDisabled(true);
            _ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(true);

            connect(_ui->buttonBack, &QPushButton::clicked, [this]{
                _ui->tabs->setCurrentIndex(0);
                _ui->buttonBack->setDisabled(true);
                _ui->buttonNext->setDisabled(false);
                _details->setItemsCheckable(_ui->listResultView->rootIndex(), false);
            });

            connect(_ui->buttonNext, &QPushButton::clicked, [this]{
                _ui->tabs->setCurrentIndex(1);
                _ui->buttonBack->setDisabled(false);
                _ui->buttonNext->setDisabled(true);
                _details->setItemsCheckable(_ui->listResultView->rootIndex(), true);
            });

            connect(_ui->buttonMoveUp, &QPushButton::clicked, this, [=]{
                auto view = _ui->listSelectedFiles;
                auto header = view->verticalHeader();
                int mapped = header->visualIndex(view->currentIndex().row());
                header->moveSection(mapped, mapped - 1);
            });

            connect(_ui->buttonMoveDown, &QPushButton::clicked, this, [=]{
                auto view = _ui->listSelectedFiles;
                auto header = view->verticalHeader();
                int mapped = header->visualIndex(view->currentIndex().row());
                header->moveSection(mapped, mapped + 1);
            });

            connect(_details, &QStandardItemModel::itemChanged, this, [=]{
                const auto checked = this->checkedResultRows();
                bool valid = selection->selectedRows().size() == checked.size();
                _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(valid);
            });

            connect(this, &QDialog::accepted, this, [=]{
                const auto result_rows = this->checkedResultRows();

                auto source_model = _ui->listSelectedFiles->model();
                auto source_header = _ui->listSelectedFiles->verticalHeader();
                QList<QPersistentModelIndex> source_rows;

                for (int i = 0; i < source_header->count(); i++) {
                    int row = source_header->logicalIndex(i);
                    source_rows << source_model->index(row, 15);
                }

                for (int i = 0; i < result_rows.size(); i++) {
                    if (i < source_rows.size()) {
                        const auto idx = result_rows[i];
                        const auto src_idx = source_rows[i];

                        // FIXME: Implement setting data!
                    }
                }
            });

            auto header = _ui->listSelectedFiles->horizontalHeader();
            for (int i = 0; i < header->count(); i++) {
                if (i != 15) {
                    header->hideSection(i);
                }
            }
        }

        TagSearchDialog::~TagSearchDialog() {
            delete _ui;
        }

        QVariantMap TagSearchDialog::resultData() const {
            return _lastResult;
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
            _ui->buttonNext->setEnabled(true);

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
                    _lastResult = job->result();

                    auto index = _details->appendResult(_lastResult);
                    _ui->listResultView->setRootIndex(index);
                    _ui->listAlbumDetails->setRootIndex(index);
                    job->deleteLater();
                });

                QThreadPool::globalInstance()->start(job);
            }
        }

        QModelIndexList TagSearchDialog::checkedResultRows() const {
            const QModelIndex root = _ui->listAlbumDetails->rootIndex();
            auto model = _ui->listAlbumDetails->model();
            return model->match(root.child(0, 0), Qt::CheckStateRole, QVariant(Qt::Checked), -1);
        }
    }
}
