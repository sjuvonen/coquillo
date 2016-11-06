
#include <QDebug>
#include <QImage>
#include <QMimeData>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

#include "tags/tagdataroles.hpp"
#include "tags/tagcontainer.hpp"
#include "imagemodel.hpp"

namespace Coquillo {
    namespace TagEditor {
        ImageModel::ImageModel(QObject * parent)
        : QAbstractTableModel(parent), _source(0) {
            _nam = new QNetworkAccessManager(this);
        }

        void ImageModel::addImage(const QImage & image) {
            // if (!image.isNull()) {
            //     MetaData::ImageList images = metaData().images();
            //     images << image;
            //     const QVariant value = QVariant::fromValue<MetaData::ImageList>(images);
            //     beginInsertRows(QModelIndex(), rowCount(), rowCount());
            //     qDebug() << "add image" << sourceModel()->setData(sourceIndex(), value);
            //     endInsertRows();
            // }
        }

        QVariant ImageModel::data(const QModelIndex & idx, int role) const {
            if (!sourceModel()) {
                return QVariant();
            }

            if (role == Qt::SizeHintRole) {
                const auto size = container().image(idx.row()).scaled(QSize(128, 128)).size();
                return size + QSize(0, 10);
            }

            if (role == Qt::DisplayRole or role == Qt::EditRole) {
                const auto file = container();
                const auto image = file.image(idx.row());

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
                const auto image = container().image(idx.row());
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
                return false;
            }
        }

        int ImageModel::rowCount(const QModelIndex & parent) const {
            if (!sourceModel() || parent.isValid()) {
                return 0;
            }

            return container().images().size();
        }

        bool ImageModel::setData(const QModelIndex & idx, const QVariant & value, int role) {

            return false;
        }

        void ImageModel::setSourceIndex(const QModelIndex & idx) {
            beginResetModel();
            // Always take the column that contains images
            _index = idx;
            endResetModel();
        }

        Tags::Container ImageModel::container() const {
            return _index.data(Tags::ContainerRole).value<Tags::Container>();
        }
    }
}
