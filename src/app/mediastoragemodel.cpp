#include "mediastoragemodel.h"
#include "mediastoragemodelcolumns.h"
#include "mediastoragemodelroles.h"
#include <QDebug>
#include <QSize>
#include <QVariant>
#include <qapplication.h>
#include <qcontainerfwd.h>
#include <qfont.h>
#include <qicon.h>
#include <qlogging.h>
#include <qnamespace.h>
#include <qpalette.h>

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

    const auto field = MediaStorageModelColumns::field(column);
    const auto media = storage->at(idx.row());

    if (role == Qt::ForegroundRole) {
        if (media.changed()) {
            const auto palette = QGuiApplication::palette();

            return palette.accent().color();
        }
    }

    if (role == FilePathRole) {
        return media.path();
    }

    if (field.isNull()) {
        if (column == MediaStorageModelColumns::pathColumn()) {
            switch (role) {
            case Qt::DisplayRole:
            case Qt::EditRole:
            case Qt::ToolTipRole:
                return media.renamed().isNull() ? media.path() : media.renamed();

            default:
                return QVariant();
            }
        }

        if (column == MediaStorageModelColumns::indicatorColumn()) {
            switch (role) {
                // case Qt::DecorationRole:
                //     return media.modified() ? QIcon::fromTheme("emblem-important") : QVariant();

                // case Qt::BackgroundRole:
                //     return media.modified() ? QGuiApplication::palette().accent().color() :
                //     QVariant();

            case Qt::DisplayRole:
                return media.changed() ? QString("⦿") : QString();

            case Qt::FontRole: {
                if (media.changed()) {
                    auto font = QApplication::font();

                    if (font.pixelSize() != -1) {
                        font.setPixelSize(font.pixelSize() * 1.6);
                    } else {
                        font.setPointSize(font.pointSize() * 1.6);
                    }

                    return font;
                }
            }

            default:
                return QVariant();
            }
        }

        if (column == 16) {
            switch (role) {
            case Qt::DisplayRole:
                return 0;

            default:
                return QVariant();
            }
        }

        qWarning() << "mediastoragemodel: Unhandled special column" << column;

        return QVariant();
    } else {
        if (role == Qt::DisplayRole || role == Qt::EditRole) {
            return media.get(field);
        }

        return QVariant();
    }
}

bool MediaStorageModel::setData(const QModelIndex &idx, const QVariant &value, int role) {
    if (role != Qt::EditRole) {
        return false;
    }

    int column = idx.column();
    const auto field = MediaStorageModelColumns::field(column);
    auto media = storage->at(idx.row());

    if (field.isNull()) {
        if (column == MediaStorageModelColumns::pathColumn()) {
            media.rename(value.toString());

            emit dataChanged(idx, idx, {Qt::EditRole, Qt::DisplayRole});
            emit dataChanged(idx.siblingAtColumn(0), idx.siblingAtColumn(0), {Qt::DisplayRole});

            return true;
        }
    } else {
        media.set(field, value.toString());

        emit dataChanged(idx.siblingAtColumn(0), idx.siblingAtColumn(0), {Qt::DisplayRole});
        emit dataChanged(idx, idx, {Qt::EditRole, Qt::DisplayRole});

        return true;
    }

    return false;
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
        beginInsertRows(QModelIndex(), first, last);
        size = last + 1;
        endInsertRows();
    }
}

} // namespace Coquillo