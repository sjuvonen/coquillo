
#include <QtConcurrent>
#include <QDebug>
#include <QFutureWatcher>
#include <QImage>
#include <QMimeData>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

#include "crawler/crawler.hpp"
#include "crawler/types.hpp"
#include "tags/image.hpp"
#include "tags/tagdataroles.hpp"
#include "tags/tagcontainer.hpp"
#include "imagemodel.hpp"

namespace Coquillo {
    namespace TagEditor {

        QVariantHash read_file(const QString & path) {
            return Crawler::FileReader().read(path, true);
        }

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

        Tags::Image ImageModel::image(int pos) const {
            return container().image(pos);
        }

        QVariant ImageModel::data(const QModelIndex & idx, int role) const {
            if (!sourceModel()) {
                return QVariant();
            }

            if (role == Qt::SizeHintRole) {
                const auto size = container().image(idx.row()).scaled(QSize(128, 128)).size();
                return size + QSize(0, 10);
            }

            if (role == Qt::DecorationRole and (idx.column() == 0 or idx.column() == 3)) {
                const auto image = container().image(idx.row());
                return image.scaled(QSize(128, 128));
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
                    case 3: {
                        const QStringList info = {
                            image.description(),
                            QString("%1x%2 px").arg(image.width()).arg(image.height()),
                            image.mimeType(),
                        };
                        return info.join('\n');
                    }
                }
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
            if (row < 0 || row + count > rowCount() || parent.isValid()) {
                return false;
            } else {
                beginRemoveRows(parent, row, row + count - 1);
                QList<Tags::Image> images = container().images();
                images.erase(images.begin() + row, images.begin() + row + count);
                sourceModel()->setData(_index, QVariant::fromValue(images));
                endRemoveRows();
                return true;
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

            initializeImages(idx);
        }

        void ImageModel::initializeImages(const QPersistentModelIndex & idx) {
            const auto file = container();

            if (file.imageCount() > 0 && file.images().size() == 0) {
                auto watcher = new QFutureWatcher<QVariantHash>(this);
                const QString path = idx.data(Tags::FilePathRole).toString();

                watcher->setFuture(QtConcurrent::run(&read_file, path));

                connect(watcher, &QFutureWatcher<QVariantHash>::finished, watcher, [this, watcher, idx]{
                    if (idx != _index) {
                        return;
                    }

                    const ImageDataList result = watcher->result().value("images").value<ImageDataList>();
                    QList<Tags::Image> images;

                    foreach (const ImageData item, result) {
                        images << Tags::Image::fromValues(item);
                    }

                    if (images.size() > 0) {
                        sourceModel()->setData(idx, QVariant::fromValue(images));
                        setSourceIndex(idx);
                    }
                    watcher->deleteLater();
                });
            }
        }


        Tags::Container ImageModel::container() const {
            return _index.data(Tags::ContainerRole).value<Tags::Container>();
        }
    }
}
