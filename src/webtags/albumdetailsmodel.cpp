
#include <QDebug>
#include <QThreadPool>
#include "albumdetailsmodel.hpp"

namespace Coquillo {
    namespace WebTags {
        AlbumDetailsModel::AlbumDetailsModel(QObject * parent)
        : QStandardItemModel(parent) {
            setColumnCount(6);
            // setHorizontalHeaderLabels({tr("Artist"), tr("Album"), tr("Disc"), tr("Date"), tr("Tracks")});
            setHorizontalHeaderLabels({tr("Title"), tr("Artist")});
        }

        QModelIndex AlbumDetailsModel::appendResult(const QVariantMap & result) {
            qDebug() << "DATA" << result;
            const QList<QVariantMap> tracks = result["tracks"].value<QList<QVariantMap> >();
            const QList<QStandardItem*> items = {
                new QStandardItem(result["id"].toString()),
                new QStandardItem(result["artist"].toString()),
                new QStandardItem(result["album"].toString()),
                new QStandardItem(result["disc"].toString()),
                new QStandardItem(result["date"].toString()),
                // new QStandardItem(tracks.size(), 3),
            };

            const QString disc_id = QString("%1::%2").arg(result["id"].toString()).arg(result["disc"].toInt());
            items.first()->setData(disc_id, DiscIdRole);

            foreach (const QVariantMap track, tracks) {
                const QList<QStandardItem*> child = {
                    // new QStandardItem(track["number"].toString()),
                    new QStandardItem(track["title"].toString()),
                    new QStandardItem(track["artist"].toString()),
                };

                items.first()->appendRow(child);
            }

            appendRow(items);

            return index(rowCount() - 1, 0);
        }

        QModelIndex AlbumDetailsModel::findResult(const QString & album_id, int disc_nr) const {
            const QString disc_id = QString("%1::%2").arg(album_id).arg(disc_nr);
            const QModelIndexList matches = match(index(0, 0), DiscIdRole, disc_id);

            return matches.value(0);
        }

        QVariant AlbumDetailsModel::headerData(int section, Qt::Orientation orientation, int role) const {
            /*
             * This is a bug fix to Qt core. Default implementation seems to return valid row number
             * only for items that have section < top level row count, therefore failing with
             * tree models who have lots of child items.
             */
            if (orientation == Qt::Vertical && role == Qt::DisplayRole) {
                return section + 1;
            }
            return QStandardItemModel::headerData(section, orientation, role);
        }

        void AlbumDetailsModel::setItemsCheckable(const QModelIndex & parent, bool state) {
            auto root = itemFromIndex(parent);

            if (root) {
                for (int i = 0; i < root->rowCount(); i++) {
                    root->child(i)->setCheckable(state);

                    if (!state) {
                        // Clear CheckState so that the view will hide checkboxes.
                        root->child(i)->setData(QVariant(), Qt::CheckStateRole);
                    }
                }
            }
        }
    }
}
