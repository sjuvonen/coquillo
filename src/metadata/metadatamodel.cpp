
#include <taglib/fileref.h>
#include <QBrush>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QIcon>
#include <QThread>

#include "mediacrawler.h"
#include "metadata.h"
#include "metadatamodel.h"
#include "filereader.h"

namespace Coquillo {
    MetaDataModel::MetaDataModel(QObject * parent)
    : QAbstractItemModel(parent), _recursive(false) {
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
//         _columns[0] = tr("Filename");
//         _columns[0] = tr("Length");

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

        qRegisterMetaType<Coquillo::MetaData>("MetaData");
    }

    MetaDataModel::~MetaDataModel() {
//         while (QThread * thread = _workers.takeFirst()) {
//             thread->terminate();
//             thread->deleteLater();
//         }
    }

    int MetaDataModel::columnCount(const QModelIndex & idx) const {
        if (idx.isValid()) {
            return 0;
        } else {
            return _columns.count();
        }
    }

    int MetaDataModel::rowCount(const QModelIndex & idx) const {
        if (idx.isValid()) {
            return 0;
        } else {
            return _metaData.count();
        }
    }

    QVariant MetaDataModel::headerData(int section, Qt::Orientation orientation, int role) const {
        if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
            return _columns[section];
        } else {
            return QVariant();
        }
    }

    QVariant MetaDataModel::data(const QModelIndex & idx, int role) const {
        switch (role) {
            case Qt::DisplayRole:
            case Qt::EditRole: {
                const MetaData meta = _metaData[idx.row()];
                const QString field = _columnMap.value(idx.column());
                return meta.value(field);
            }

            case Qt::DecorationRole: {
                if (idx.column() == 0 && isRowChanged(idx)) {
                    return QIcon::fromTheme("emblem-important");
                }
                break;
            }

            case Qt::ForegroundRole: {
                if (isRowChanged(idx)) {
                    return QBrush(Qt::red);
                }
                break;
            }

            case Qt::SizeHintRole:
                return QSize(100, 24);

            case ModifiedRole: {
                return isChanged(idx);
            }

            case FieldNameRole:
                return _columnMap.value(idx.column());
        }

        return QVariant();
    }

    bool MetaDataModel::setData(const QModelIndex & idx, const QVariant & value, int role) {
        if (role != Qt::EditRole && role != Qt::DisplayRole)  {
            return false;
        }

        if (!idx.isValid() || idx.model() != this) {
            return false;
        }

        const int row = idx.row();
        const QString name = _columnMap.value(idx.column());
        MetaData meta = _metaData[row];


        bool int_changed = value.type() == QVariant::Int && (value.toInt() != meta[name].toInt());
        bool str_changed = value.type() != QVariant::Int && (value.toString() != meta[name].toString());
//         if (meta.value(name).toString() != value.toString()) {
        if (int_changed || str_changed) {
            backup(_metaData[row]);
            _metaData[row].insert(name, value);
            rowChanged(idx);
            qDebug() << QString("set %1 to").arg(name) << value;
            return true;
        } else {
            return false;
        }
    }

    void MetaDataModel::backup(const MetaData & metaData) {
        if (!_original.contains(metaData.path())) {
            _original[metaData.path()] = metaData;
        }
    }

    QModelIndex MetaDataModel::index(int row, int col, const QModelIndex & parent) const {
        if (parent.isValid()) {
            return QModelIndex();
        } else {
            const QString path = _metaData.value(row).path();
            return createIndex(row, col, qHash(path, col));
        }
    }

    MetaData MetaDataModel::metaData(int row, bool original) const {
        const MetaData meta = _metaData.value(row);
        if (original && _original.contains(meta.path())) {
            return _original[meta.path()];
        } else {
            return meta;
        }
    }

    void MetaDataModel::addDirectory(const QString & dir) {
        _directories << dir;
//         QThread * worker = createWorker();
        MediaCrawler * crawler = createCrawler();
//         crawler->setDirectory(dir);
//         crawler->moveToThread(worker);
        connect(crawler, SIGNAL(finished(QStringList)), SLOT(addFiles(QStringList)));
        connect(crawler, SIGNAL(finished(QStringList)), crawler, SLOT(deleteLater()));
//         connect(worker, SIGNAL(started()), crawler, SLOT(start()));
        crawler->crawl(dir);
//         worker->start();
    }

    bool MetaDataModel::removeRows(int row, int count, const QModelIndex & parent) {
        if (row < 0 || rowCount() <= row) {
            return false;
        }

        count = qMin(row + count, _metaData.count()) - row;
        beginRemoveRows(parent, row, row + count - 1);
        while (count-- > 0) {
            _metaData.removeAt(row);
        }
        endRemoveRows();
        return true;
    }

    void MetaDataModel::addFiles(const QStringList & files) {
        QThread * worker = createWorker();
        foreach (QString file, files) {
            FileReader * reader = FileReader::create(file);
            reader->moveToThread(worker);
            if (reader) {
                connect(reader, SIGNAL(finished(MetaData)), SLOT(addMetaData(MetaData)));
                connect(reader, SIGNAL(finished(MetaData)), reader, SLOT(deleteLater()));
                connect(worker, SIGNAL(started()), reader, SLOT(start()));
            }
        }
        worker->start();
    }

    void MetaDataModel::clear() {
        beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
        _metaData.clear();
        _original.clear();
        _directories.clear();
        endRemoveRows();
    }

    void MetaDataModel::removeDirectory(const QString & dir) {
        for (int i = rowCount() - 1; i >= 0; i--) {
            if (containedDirectoryForRow(i) == dir) {
                removeRow(i);
            }
        }
        _directories.removeOne(dir);
    }

    void MetaDataModel::revert() {
        qDebug() << "revert";
        beginResetModel();
        for (int i = 0; i < rowCount(); i++) {
            _metaData[i] = metaData(i, true);
        }
        _original.clear();
        endResetModel();
    }

    void MetaDataModel::revert(const QModelIndex & idx) {
        if (idx.isValid()) {
            int row = idx.row();
            const QString path = metaData(row).path();
            _metaData[row] = metaData(row, true);
            _original.remove(path);
            rowChanged(idx);
        }
    }

    void MetaDataModel::addMetaData(const MetaData & metaData) {
        int row = rowCount();
        beginInsertRows(QModelIndex(), row, row);
        _metaData << metaData;
        endInsertRows();
    }

    QStringList MetaDataModel::nameFilters() {
        TagLib::StringList exts = TagLib::FileRef::defaultFileExtensions();
        QStringList types = QString::fromUtf8(exts.toString("%%*.").toCString(true)).split("%%");
        types << QString("*.%1").arg(types.takeFirst());
        return types;
    }

    bool MetaDataModel::isRowChanged(const QModelIndex & idx) const {
        const MetaData data = _metaData[idx.row()];
        return _original.contains(data.path());
    }

    bool MetaDataModel::isChanged(const QModelIndex & idx) const {
        const MetaData data = _metaData[idx.row()];
        if (_original.contains(data.path())) {
            const QString key = idx.data(FieldNameRole).toString();
            const MetaData original = _original[data.path()];
            return data.value(key) != original.value(key);
        } else {
            return false;
        }
    }

    void MetaDataModel::rowChanged(const QModelIndex & idx) {
        emit dataChanged(idx.sibling(idx.row(), 0), idx.sibling(idx.row(), columnCount() - 1));
    }

    QString MetaDataModel::containedDirectoryForRow(int row) const {
        QDir dir = QFileInfo(metaData(row).path()).absoluteDir();
        do {
            if (_directories.contains(dir.absolutePath())) {
                return dir.absolutePath();
            }
        } while (dir.cdUp());

        return QString();
    }

    MediaCrawler * MetaDataModel::createCrawler() {
        MediaCrawler * crawler = new MediaCrawler;
        crawler->setNameFilters(nameFilters());
        crawler->setRecursive(isRecursive());
        return crawler;
    }

    QThread * MetaDataModel::createWorker() {
        QThread * worker = new QThread;
//         _workers << worker;
        return worker;
    }
}
