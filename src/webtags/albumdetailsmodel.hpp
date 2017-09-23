#ifndef COQUILLO_WEBTAGS_AlbumDetailsModel_H
#define COQUILLO_WEBTAGS_AlbumDetailsModel_H

#include <QStandardItemModel>

namespace Coquillo {
    namespace WebTags {
        class AlbumDetailsModel : public QStandardItemModel {
            Q_OBJECT

            public:
                enum ItemDataRole { DiscIdRole = Qt::UserRole + 1};

                AlbumDetailsModel(QObject * parent = nullptr);

                QModelIndex appendResult(const QVariantMap & result);
                QModelIndex findResult(const QString & album_id, int disc_nr = 1) const;

                QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

        };
    }
}

#endif
