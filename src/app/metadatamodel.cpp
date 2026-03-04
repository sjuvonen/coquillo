#include "metadatamodel.h"
#include <qabstractitemmodel.h>
#include <qhash.h>
#include <qlogging.h>
#include <qnamespace.h>
#include <qvariant.h>

namespace Coquillo {
MetadataModel::MetadataModel(QObject *parent) : QAbstractItemModel(parent), mediaRow(-1) {}

void MetadataModel::setStorage(MediaStorage *mediaStorage) { storage = mediaStorage; }

void MetadataModel::setRow(int row) {
    beginResetModel();

    mediaRow = row;

    endResetModel();
}

void MetadataModel::setTag(const QString &tag) {
    beginResetModel();

    mediaTag = tag;

    endResetModel();
}

int MetadataModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) {
        return 0;
    }

    if (mediaRow == -1) {
        return 0;
    }

    const auto media = storage->at(mediaRow);
    const auto tags = media.tags()[mediaTag];

    return tags.size();
}

int MetadataModel::columnCount(const QModelIndex &parent) const { return 2; }

QModelIndex MetadataModel::index(int row, int col, const QModelIndex &parent) const {
    if (!parent.isValid()) {
        return createIndex(row, col);
    } else {
        return QModelIndex();
    }
}

QVariant MetadataModel::data(const QModelIndex &idx, int role) const {
    if (!idx.isValid()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        const auto media = storage->at(mediaRow);
        const auto tags = media.tags()[mediaTag];

        if (idx.column() == 0) {
            return tags.keys()[idx.row()];
        }

        if (idx.column() == 1) {
            return tags.values()[idx.row()];
        }
    }

    return QVariant();
}

bool MetadataModel::setData(const QModelIndex &idx, const QVariant &value, int role) {
    return false;
};

QVariant MetadataModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return section == 0 ? tr("Tag") : tr("Value");
    }

    return QVariant();
};

QModelIndex MetadataModel::parent(const QModelIndex &idx) const { return QModelIndex(); }
} // namespace Coquillo