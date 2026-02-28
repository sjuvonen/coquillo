#include "mediastoragemodel.h"
#include "mediastoragemodelcolumns.h"
#include <QDebug>
#include <QSize>
#include <QVariant>

namespace Coquillo {
MediaStorageModel::MediaStorageModel(QObject *parent) : QAbstractItemModel(parent), size(0) {}

void MediaStorageModel::setStorage(MediaStorage *storage) {
    this->storage = QPointer(storage);

    connect(storage, &MediaStorage::progress, this, &MediaStorageModel::update);
}

int MediaStorageModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) {
        return 0;
    } else {
        return size;
    }
}

int MediaStorageModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid()) {
        return 0;
    } else {
        return MediaStorageModelColumns::size();
    }
}
QModelIndex MediaStorageModel::index(int row, int col, const QModelIndex &parent) const {
    if (row < 0 || row >= rowCount() || col < 0 || col >= columnCount()) {
        return QModelIndex();
    } else {
        return createIndex(row, col, qHash(storage->at(row).path()));
    }
}

QVariant MediaStorageModel::data(const QModelIndex &idx, int role) const {
    if (!idx.isValid()) {
        return QVariant();
    }

    /**
     * NOTE: Sanity check in case rows have just been removed and we didn't yet
             catch it.
     */
    if (idx.row() >= storage->size()) {
        return QVariant();
    }

    int column = idx.column();

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        const auto field = MediaStorageModelColumns::field(column);
        const auto media = storage->at(idx.row());

        if (field.isNull()) {
            switch (column) {
            case 0:
                // FIXME: Modification status.
                return QVariant();

            case 1:
                return media.path();

            case 16:
                return 0;

            default:
                qWarning() << "mediastoragemodel: Unhandled special column" << column;

                return QVariant();
            }
        } else {
            return media.get(field);
        }

    } else if (role == Qt::SizeHintRole) {
        return QSize(200, 0);
    } else {
        return QVariant();
    }
}

QVariant MediaStorageModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Vertical) {
        return section + 1;
    }

    if (section < 0 || section >= MediaStorageModelColumns::size()) {
        return QVariant();
    }

    return MediaStorageModelColumns::label(section);
}

void MediaStorageModel::update() {
    int first = rowCount();
    int last = storage->size() - 1;

    if (last > 0) {
        qDebug() << "update model" << first << last;

        beginInsertRows(QModelIndex(), first, last);
        size = last;
        endInsertRows();
    }
}

} // namespace Coquillo