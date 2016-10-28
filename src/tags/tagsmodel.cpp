#include "crawler/crawler.hpp"
#include "tag.hpp"
#include "tagsmodel.hpp"

#include <QDebug>

namespace Coquillo {
    namespace Tags {
        TagsModel::TagsModel(QObject * parent)
        : QAbstractItemModel(parent) {
            _columns = QHash<int, QString>();
            _columns[0] = " ";
            _columns[1] = tr("Title");
            _columns[2] = tr("Artist");
            _columns[3] = tr("Album");
            _columns[4] = tr("Genre");
            _columns[5] = tr("Comment");
            _columns[6] = tr("Year");
            _columns[7] = tr("#");
            _columns[8] = tr("Total");
            _columns[9] = tr("Disc");
            _columns[10] = tr("Original Artist");
            _columns[11] = tr("Album Artist");
            _columns[12] = tr("Composer");
            _columns[13] = tr("Url");
            _columns[14] = tr("Encoder");
            _columns[15] = tr("Filename");
            _columns[16] = tr("Images");

            _columnMap = QHash<int, QString>();
            _columnMap[1] = "title";
            _columnMap[2] = "artist";
            _columnMap[3] = "album";
            _columnMap[4] = "genre";
            _columnMap[5] = "comment";
            _columnMap[6] = "year";
            _columnMap[7] = "number";
            _columnMap[8] = "total";
            _columnMap[9] = "disc";
            _columnMap[10] = "original_artist";
            _columnMap[11] = "album_artist";
            _columnMap[12] = "composer";
            _columnMap[13] = "url";
            _columnMap[14] = "encoder";
            _columnMap[15] = "filename";
            _columnMap[16] = "images";

            qRegisterMetaType<Coquillo::Tags::Tag>("Tag");

            auto crawler = new Crawler::Crawler(this);
            crawler->searchPath("/mnt/data/Music/Artists/666");

            crawler->connect(crawler, &Crawler::Crawler::finished, [this, crawler]{
                qDebug() << "size:" << rowCount();
                crawler->deleteLater();
            });

            crawler->connect(crawler, &Crawler::Crawler::results, [this](const QList<QVariantHash> & results) {
                qDebug() << results;
                beginInsertRows(QModelIndex(), _store.size(), _store.size() + results.size() - 1);
                _store.add(results);
                endInsertRows();
            });
        }

        int TagsModel::columnCount(const QModelIndex & parent) const {
            return parent.isValid() ? 0 : _columns.size();
        }

        int TagsModel::rowCount(const QModelIndex & parent) const {
            return parent.isValid() ? 0 : _store.size();
        }

        QVariant TagsModel::data(const QModelIndex & idx, int role) const {
            if (idx.isValid() && (role == Qt::EditRole || role == Qt::DisplayRole)) {
                const auto file = _store.at(idx.row());

                switch (idx.column()) {
                    case 14:
                        return file.path();

                    default: {
                        const auto field = _columnMap.value(idx.column() + 1);
                        return file.value(field);
                    }
                }
            }

            return QVariant();
        }

        QVariant TagsModel::headerData(int section, Qt::Orientation orientation, int role) const {
            if (orientation == Qt::Horizontal && (role == Qt::EditRole || role == Qt::DisplayRole)) {
                return _columns.value(section + 1);
            }
            return QAbstractItemModel::headerData(section, orientation, role);
        }

        QModelIndex TagsModel::index(int row, int col, const QModelIndex & parent) const {
            if (parent.isValid()) {
                return QModelIndex();
            } else {
                return createIndex(row, col, qHash(_store.at(row).id(), col));
            }
        }
    }
}
