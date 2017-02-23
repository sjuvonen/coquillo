
#include <QDebug>
#include <QDataWidgetMapper>
#include <QFileDialog>
#include "tags/imagetypes.hpp"
#include "tags/tagdataroles.hpp"
#include "tags/tagsmodel.hpp"
#include "imagemodel.hpp"
#include "imagetags.hpp"
#include "ui_imagetags.h"

namespace Coquillo {
    namespace TagEditor {
        ImageTags::ImageTags(QWidget * parent)
        : EditorPageBase(parent) {
            _ui = new Ui::ImageTags;
            _ui->setupUi(this);

            _ui->imageType->addItems(Tags::ImageTypes::types());

            _images = new ImageModel;
            _ui->listImages->setModel(_images);
            _ui->listImages->setModelColumn(3);

            _mapper = new QDataWidgetMapper(this);
            _mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
            _mapper->setModel(_images);
            _mapper->addMapping(_ui->imageDescription, 0);
            _mapper->addMapping(_ui->imageType, 1, "currentIndex");

            connect(_ui->listImages->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)),
                SLOT(onImageSelect(QModelIndex)));

            connect(_ui->imageType, SIGNAL(currentIndexChanged(int)), _mapper, SLOT(submit()));
            connect(_ui->imageDescription, SIGNAL(textChanged(QString)), _mapper, SLOT(submit()));

            connect(_images, SIGNAL(rowsInserted(QModelIndex, int, int)), SLOT(ensureRowSelected()));            connect(_ui->listImages->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), SLOT(ensureRowSelected()));
        }

        ImageTags::~ImageTags() {
            delete _ui;
        }

        void ImageTags::on_buttonAdd_clicked() {
            const QStringList files = QFileDialog::getOpenFileNames(this,
                tr("Import images"), _last_dir);

            foreach (const QString & path, files) {
                const QImage image(path);
                if (!image.isNull()) {
                    imageModel()->addRawImage(image);
                }
                _last_dir = QFileInfo(path).absolutePath();
            }
        }

        void ImageTags::on_buttonExport_clicked() {
            // TODO: Support multiple images at once

            if (!currentIndex().isValid()) {
                return;
            }

            const auto image = _images->image(currentRow());
            const auto filename = QFileDialog::getSaveFileName(this, tr("Export image to"), _last_dir);

            if (!filename.isEmpty()) {
                image.source().save(filename);
            }
        }

        void ImageTags::on_buttonDelete_clicked() {
            // TODO: Support multiple rows at once
            _images->removeRow(currentRow());
        }

        void ImageTags::on_buttonCloneImages_clicked() {
            const QModelIndex index = imageModel()->sourceIndex();
            const QVariant value = index.data(Tags::ImageDataRole);

            foreach (const QModelIndex idx, selectionModel()->selectedRows(index.column())) {
                model()->setData(idx, value);
            }
        }

        void ImageTags::setModel(QAbstractItemModel * model) {
            EditorPageBase::setModel(model);
            _images->setSourceModel(model);
        }

        void ImageTags::setEditorIndex(const QModelIndex & idx) {
            EditorPageBase::setEditorIndex(idx);

            const auto image_idx = idx.sibling(idx.row(), Tags::TagsModel::ImageField);
            _images->setSourceIndex(image_idx);
            _ui->listImages->setCurrentIndex(_images->index(0, 0));

            _ui->imageType->setEnabled(image_idx.isValid());
            _ui->imageDescription->setEnabled(image_idx.isValid());
            _ui->buttonDelete->setEnabled(image_idx.isValid());

            if (idx.isValid()) {
                const auto path = idx.sibling(idx.row(), Tags::TagsModel::PathField).data(Qt::EditRole).toString();
                _last_dir = QFileInfo(path).absolutePath();
            } else {
                _last_dir.clear();
            }
        }

        void ImageTags::onImageSelect(const QModelIndex & idx) {
            _mapper->setCurrentModelIndex(idx);
        }

        QModelIndex ImageTags::currentIndex() const {
            return _ui->listImages->currentIndex();
        }

        int ImageTags::currentRow() const {
            return currentIndex().row();
        }

        void ImageTags::ensureRowSelected() {
            if (currentRow() == -1) {
                int column = _ui->listImages->modelColumn();
                _ui->listImages->setCurrentIndex(_images->index(0, column));
            }
        }

        int ImageTags::imageCount() const {
            return _ui->listImages->model()->rowCount();
        }
    }
}
