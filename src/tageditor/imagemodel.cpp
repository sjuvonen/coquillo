
#include <QDebug>

#include "metadata/metadatamodel.h"
#include "imagemodel.h"

namespace Coquillo {
    namespace TagEditor {
        ImageModel::ImageModel(QObject * parent)
        : QAbstractTableModel(parent) {

        }

        QVariant ImageModel::data(const QModelIndex & idx, int role) const {
            if (role == Qt::DisplayRole or role == Qt::EditRole) {
                const MetaData::ImageList images = metaData().images();
                const MetaData::Image & image = images[idx.row()];

                switch (idx.column()) {
                    case 0:
                        return image.description();
                    case 1:
                        return image.type();
                    case 2:
                        return image.mimeType();
                    case 3:
                        return image.description()
                            + QString("\n%1x%2 px").arg(image.width()).arg(image.height())
                            + image.mimeType().prepend("\n");
                }
            }

            if (role == Qt::DecorationRole and (idx.column() == 0 or idx.column() == 3)) {
                const MetaData::Image & image = metaData().images()[idx.row()];
                return image.scaled(QSize(128, 128));
            }

            return QVariant();
        }

        QVariant ImageModel::headerData(int section, Qt::Orientation orientation, int role) const {
            if ((role == Qt::DisplayRole or role == Qt::EditRole) and orientation == Qt::Horizontal) {
                return ((QStringList){tr("Image"), tr("Type"), tr("Description")})[section];
            }
            return QVariant();
        }

        int ImageModel::rowCount(const QModelIndex & parent) const {
            if (parent.isValid()) {
                return 0;
            } else {
                return metaData().images().count();
            }
        }

        void ImageModel::setSourceIndex(const QModelIndex & idx) {
            beginResetModel();
            _source = idx;
            endResetModel();
        }

        MetaData::MetaData ImageModel::metaData() const {
            int role = MetaData::MetaDataModel::MetaDataRole;
            return _source.data(role).value<MetaData::MetaData>();
        }
    }
}
