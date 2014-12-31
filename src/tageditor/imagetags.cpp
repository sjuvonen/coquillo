
#include <QDebug>
#include <QDataWidgetMapper>
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
            _mapper->setModel(_model);
            _mapper->addMapping(_ui->imageDescription, 0);
            _mapper->addMapping(_ui->imageType, 1, "currentIndex");

            connect(_ui->listImages->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)),
                SLOT(onImageSelect(QModelIndex)));
        }

        ImageTags::~ImageTags() {
            delete _ui;
        }

        int ImageTags::imageCount() const {
            return _ui->listImages->model()->rowCount();
        }

        void ImageTags::setEditorIndex(const QModelIndex & idx) {
            _model->setSourceIndex(idx);
            const QModelIndex imgidx = _model->index(0, 3);
            _ui->listImages->setCurrentIndex(imgidx);
            _ui->imageType->setEnabled(imgidx.isValid());
            _ui->imageDescription->setEnabled(imgidx.isValid());
            _ui->buttonDelete->setEnabled(imgidx.isValid());
        }

        void ImageTags::on_cloneImages_clicked() {
            qDebug() << "field cloning not yet implemented!";
        }

        void ImageTags::onImageSelect(const QModelIndex & idx) {
            _mapper->setCurrentModelIndex(idx);
        }

        MetaData::MetaData ImageTags::metaData(const QModelIndex & idx) const {
            int role = MetaData::MetaDataModel::MetaDataRole;
            return idx.data(role).value<MetaData::MetaData>();
        }
    }
}
