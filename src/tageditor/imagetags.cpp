
#include <QDebug>
#include <QDataWidgetMapper>
#include <QFileDialog>
#include "metadata/imagetypes.h"
#include "metadata/metadatamodel.h"
#include "imagemodel.h"
#include "imagetags.h"
#include "ui_imagetags.h"

namespace Coquillo {
    namespace TagEditor {
        ImageTags::ImageTags(QWidget * parent)
        : QWidget(parent) {
            _ui = new Ui::ImageTags;
            _ui->setupUi(this);

            _ui->imageType->addItems(MetaData::ImageTypes::types());

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

        void ImageTags::addImage() {
            const MetaData::MetaData data = metaData();
            const QStringList files = QFileDialog::getOpenFileNames(this,
                tr("Import images"), _last_dir);

            foreach (const QString & path, files) {
                _last_dir = QFileInfo(path).absolutePath();
                const QImage image(path);

                if (!image.isNull()) {
                    imageModel()->addImage(image);
                }
            }
        }

        void ImageTags::exportCurrentImage() {
            if (!_ui->listImages->currentIndex().isValid()) {
                return;
            }
            const MetaData::ImageList images = metaData().images();
            const MetaData::Image image = images[_ui->listImages->currentIndex().row()];
            const QString filename = QFileDialog::getSaveFileName(this,
                tr("Export image to"), _last_dir);

            if (!filename.isEmpty()) {
                image.source().save(filename);
            }
        }

        int ImageTags::imageCount() const {
            return _ui->listImages->model()->rowCount();
        }

        MetaData::MetaData ImageTags::metaData() const {
            int role = MetaData::MetaDataModel::MetaDataRole;
            return imageModel()->sourceIndex().data(role).value<MetaData::MetaData>();
        }

        QAbstractItemModel * ImageTags::model() const {
            return _model->sourceModel();
        }

        void ImageTags::removeCurrentRow() {
            qDebug() << "remove row" << _ui->listImages->currentIndex().row() <<
            _model->removeRow(_ui->listImages->currentIndex().row());
        }

        void ImageTags::setModel(QAbstractItemModel * model) {
            _model->setSourceModel(model);
        }

        void ImageTags::setEditorIndex(const QModelIndex & idx) {
            _model->setSourceIndex(idx);
            const QModelIndex imgidx = _model->index(0, 3);
            _ui->listImages->setCurrentIndex(imgidx);
            _ui->imageType->setEnabled(imgidx.isValid());
            _ui->imageDescription->setEnabled(imgidx.isValid());
            _ui->buttonDelete->setEnabled(imgidx.isValid());

            if (idx.isValid()) {
                _last_dir = QFileInfo(metaData().path()).absolutePath();
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
    }
}
