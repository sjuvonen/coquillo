
#include <QDebug>
#include <QMimeData>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

#include "metadata/metadatamodel.hpp"
#include "imagemodel.hpp"

namespace Coquillo {
    namespace TagEditor {
        ImageModel::ImageModel(QObject * parent)
        : QAbstractTableModel(parent), _source(0) {
            _nam = new QNetworkAccessManager(this);
        }

        void ImageModel::addImage(const QImage & image) {
            if (!image.isNull()) {
                MetaData::ImageList images = metaData().images();
                images << image;
                const QVariant value = QVariant::fromValue<MetaData::ImageList>(images);
                beginInsertRows(QModelIndex(), rowCount(), rowCount());
                qDebug() << "add image" << sourceModel()->setData(sourceIndex(), value);
                endInsertRows();
            }
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
                const MetaData::Image image = metaData().images()[idx.row()];
                return image.scaled(QSize(128, 128));
            }

            return QVariant();
        }

        void ImageModel::download(const QUrl & url) {
            QNetworkReply * reply = _nam->get(QNetworkRequest(url));

            connect(reply, &QNetworkReply::finished, [=]() {
                if (reply->isReadable()) {
                    addImage(QImage::fromData(reply->readAll()));
                    reply->deleteLater();
                } else {
                    qWarning() << "Failed to read data" << reply->error();
                }
            });
        }

        bool ImageModel::dropMimeData(const QMimeData * data, Qt::DropAction, int, int, const QModelIndex &) {
            foreach (const QUrl & url, data->urls()) {
                if (url.isLocalFile()) {
                    addImage(QImage(url.path()));
                } else {
                    download(url);
                }
            }

            if (data->hasImage()) {
                addImage(qvariant_cast<QImage>(data->imageData()));
            }

            return false;
        }

        Qt::ItemFlags ImageModel::flags(const QModelIndex & idx) const {
            return QAbstractTableModel::flags(idx) | Qt::ItemIsDropEnabled;
        }

        QVariant ImageModel::headerData(int section, Qt::Orientation orientation, int role) const {
            if ((role == Qt::DisplayRole or role == Qt::EditRole) and orientation == Qt::Horizontal) {
                return ((QStringList){tr("Image"), tr("Type"), tr("Description")})[section];
            }
            return QVariant();
        }

        QStringList ImageModel::mimeTypes() const {
            return { "text/uri-list", "image/png", "image/jpeg", "image/gif" };
        }

        bool ImageModel::removeRows(int row, int count, const QModelIndex & parent) {
            if (row < 0 || parent.isValid()) {
                return false;
            } else {
                beginRemoveRows(parent, row, row+count-1);
                MetaData::ImageList images = metaData().images();
                images.removeAt(row);
                const QVariant value = QVariant::fromValue<MetaData::ImageList>(images);
                bool ok = sourceModel()->setData(sourceIndex(), value);
                endRemoveRows();
                return ok;
            }
        }

        int ImageModel::rowCount(const QModelIndex & parent) const {
            if (parent.isValid()) {
                return 0;
            } else {
                return metaData().images().count();
            }
        }

        bool ImageModel::setData(const QModelIndex & idx, const QVariant & value, int role) {
            if (!sourceModel()) {
                return false;
            }

            if (role == Qt::DisplayRole or role == Qt::EditRole) {
                MetaData::ImageList images = metaData().images();
                MetaData::Image & image = images[idx.row()];

                switch (idx.column()) {
                    case 0:
                        image.setDescription(value.toString());
                        break;
                    case 1:
                        image.setType(value.toInt());
                        break;
                    case 2:
                        image.setMimeType(value.toString());
                        break;
                    default:
                        return false;
                }

                const QVariant value = QVariant::fromValue<MetaData::ImageList>(images);
                return _source->setData(sourceIndex(), value);
            }

            return false;
        }

        void ImageModel::setSourceIndex(const QModelIndex & idx) {
            beginResetModel();
            // Always take the column that contains images
            _index = idx.sibling(idx.row(), 16);
            endResetModel();
        }

        MetaData::MetaData ImageModel::metaData() const {
            int role = MetaData::MetaDataModel::MetaDataRole;
            return qvariant_cast<MetaData::MetaData>(_index.data(role));
        }
    }
}
