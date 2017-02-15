
#include <QDebug>
#include <QDataWidgetMapper>
#include <QFileDialog>
#include "tags/imagetypes.hpp"
#include "tags/tagsmodel.hpp"
#include "imagemodel.hpp"
#include "imagetags.hpp"
#include "ui_imagetags.h"

namespace Coquillo {
    namespace TagEditor {
        ImageTags::ImageTags(QWidget * parent)
        : QWidget(parent) {
            _ui = new Ui::ImageTags;
            _ui->setupUi(this);

            _ui->imageType->addItems(Tags::ImageTypes::types());

            _model = new ImageModel;
            _ui->listImages->setModel(_model);
            _ui->listImages->setModelColumn(3);

            _mapper = new QDataWidgetMapper(this);
            _mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
            _mapper->setModel(_model);
            _mapper->addMapping(_ui->imageDescription, 0);
            _mapper->addMapping(_ui->imageType, 1, "currentIndex");

            connect(_ui->listImages->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)),
                SLOT(onImageSelect(QModelIndex)));

            connect(_ui->imageType, SIGNAL(currentIndexChanged(int)), _mapper, SLOT(submit()));
            connect(_ui->imageDescription, SIGNAL(textChanged(QString)), _mapper, SLOT(submit()));
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
                    imageModel()->addRawImage(image, path);
                }
                _last_dir = QFileInfo(path).absolutePath();
            }
        }

        void ImageTags::on_buttonExport_clicked() {
            // TODO: Support multiple images at once

            if (!currentIndex().isValid()) {
                return;
            }

            const auto image = _model->image(currentRow());
            const auto filename = QFileDialog::getSaveFileName(this, tr("Export image to"), _last_dir);

            if (!filename.isEmpty()) {
                image.source().save(filename);
            }
        }

        int ImageTags::imageCount() const {
            return _ui->listImages->model()->rowCount();
        }

        QAbstractItemModel * ImageTags::model() const {
            return _model->sourceModel();
        }

        void ImageTags::on_buttonDelete_clicked() {
            // TODO: Support multiple rows at once
            _model->removeRow(currentRow());
        }

        void ImageTags::setModel(QAbstractItemModel * model) {
            _model->setSourceModel(model);
        }

        void ImageTags::setEditorIndex(const QModelIndex & idx) {
            const auto image_idx = idx.sibling(idx.row(), Tags::TagsModel::ImageField);
            _model->setSourceIndex(image_idx);
            _ui->listImages->setCurrentIndex(_model->index(0, 0));

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

        void ImageTags::on_cloneImages_clicked() {
            qDebug() << "field cloning not yet implemented!";
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
    }
}
