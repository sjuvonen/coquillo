#include <QBrush>
#include <QDir>
#include <QIcon>
#include "../progresslistener.hpp"
#include "crawler/crawler.hpp"
#include "tag.hpp"
#include "tagdataroles.hpp"
#include "tagsmodel.hpp"

#include <QDateTime>
#include <QDebug>
#include <QThread>

namespace Coquillo {
    namespace Tags {
        TagsModel::TagsModel(Store * store, ProgressListener * progress, QObject * parent)
        : QAbstractItemModel(parent), _store(store), _progress(progress), _recursive(false) {
            _labels = QStringList({
                "",
                tr("Title"),
                tr("Artist"),
                tr("Album"),
                tr("Genre"),
                tr("Comment"),
                tr("Year"),
                tr("#"),
                tr("Total"),
                tr("Disc"),
                tr("Original Artist"),
                tr("Album Artist"),
                tr("Composer"),
                tr("URL"),
                tr("Encoder"),
                tr("Filename"),
                tr("Images"),
            });

            _fields = QStringList({
                QString(),
                "title",
                "artist",
                "album",
                "genre",
                "comment",
                "year",
                "number",
                "total",
                "disc",
                "original_artist",
                "album_artist",
                "composer",
                "url",
                "encoder",
                "filename",
                "images",
            });

            qRegisterMetaType<Coquillo::Tags::Container>("container");

            connect(_store, &Store::committed, [this] {
                emit dataChanged(index(0, 0), index(rowCount()-1, columnCount()-1), {ItemModifiedStateRole});
            });
        }

        int TagsModel::columnCount(const QModelIndex & parent) const {
            return parent.isValid() ? 0 : _labels.size();
        }

        int TagsModel::rowCount(const QModelIndex & parent) const {
            return parent.isValid() ? 0 : _store->size();
        }

        QVariant TagsModel::data(const QModelIndex & idx, int role) const {
            if (idx.isValid()) {
                switch (role) {
                    case Qt::DisplayRole:
                    case Qt::EditRole: {
                        const Container file = _store->at(idx.row());
                        if (idx.column() == PathField) {
                            if (role == Qt::EditRole) {
                                return file.path();
                            } else {
                                const auto root = containedDirectoryForRow(idx.row());
                                const int root_length = root.length() ? root.length() + 1 : 0;
                                return file.path().midRef(root_length).toString();
                            }
                        } else if (idx.column() == ImageField) {
                            return QString("%1 images").arg(file.imageCount());
                        }
                        const auto field = _fields[idx.column()];
                        return file.value(field);
                    }


                    case Qt::DecorationRole:
                        if (idx.column() == FeedbackField && isRowChanged(idx)) {
                            return QIcon::fromTheme("emblem-important");
                        }
                        break;

                    case Qt::ForegroundRole:
                        if (isRowChanged(idx)) {
                            return QBrush(Qt::red);
                        }
                        break;

                    case ContainerRole:
                        return QVariant::fromValue<Container>(_store->at(idx.row()));

                    case ImageDataRole:
                        return QVariant::fromValue(_store->at(idx.row()).images());

                    case ItemModifiedStateRole:
                        return _store->isModified(idx.row());

                    case FieldModifiedStateRole:
                        return _store->isFieldModified(idx.row(), _fields[idx.column()]);

                    case RootPathRole:
                        return containedDirectoryForRow(idx.row());

                    case FilePathRole:
                      return data(index(idx.row(), PathField), Qt::EditRole);

                    case FileNameRole:
                        return QFileInfo(data(index(idx.row(), PathField), Qt::EditRole).toString()).fileName();

                    case ValuesMapRole: {
                        QVariantHash data;
                        const auto file = _store->at(idx.row());

                        for (auto i = _fields.constBegin() + 1; i != _fields.constEnd(); i++) {
                            if (*i == "filename") {
                                data[*i] = file.path();
                            } else {
                                data[*i] = file.value(*i);
                            }
                        }
                        return data;
                    }

                }
            }

            return QVariant();
        }

        bool TagsModel::setData(const QModelIndex & idx, const QVariant & value, int role) {
            if (!idx.isValid()) {
                return false;
            }

            if (role == Qt::EditRole || role == Qt::DisplayRole) {
                if (idx.column() == ImageField) {
                    if (_store->setImages(idx.row(), value.value<QList<Image> >())) {
                        rowChanged(idx);
                        return true;
                    }
                } else if (idx.column() == PathField) {
                    if (_store->rename(idx.row(), value.toString())) {
                        rowChanged(idx);
                        return true;
                    }
                } else {
                    const QString field = _fields[idx.column()];
                    if (_store->setValue(idx.row(), field, value)) {
                        rowChanged(idx);
                        return true;
                    }
                }
            } else if (role == ValuesMapRole) {
                const QVariantHash values = value.value<QVariantHash>();
                bool changed = false;

                for (auto i = values.begin(); i != values.end(); i++) {
                    if (_store->setValue(idx.row(), i.key(), i.value())) {
                        changed = true;
                    }
                }

                rowChanged(idx);
                return changed;
            } else if (role == FilePathRole) {
                return _store->rename(idx.row(), value.toString());
            } else {
                qWarning() << "Model does not support setting data for role" << role;
            }

            return QAbstractItemModel::setData(idx, value, role);
        }

        QVariant TagsModel::headerData(int section, Qt::Orientation orientation, int role) const {
            if (orientation == Qt::Horizontal && (role == Qt::EditRole || role == Qt::DisplayRole)) {
                return _labels.value(section);
            }
            return QAbstractItemModel::headerData(section, orientation, role);
        }

        QModelIndex TagsModel::index(int row, int col, const QModelIndex & parent) const {
            if (row >= 0 && row < rowCount() && col >= 0 && col < columnCount() && !parent.isValid()) {
                return createIndex(row, col, qHash(_store->at(row).id(), col));
            } else {
                return QModelIndex();
            }
        }

        bool TagsModel::removeRows(int row, int count, const QModelIndex & parent) {
            if (parent.isValid() || row < 0 || row >= rowCount()) {
                return false;
            }

            count = qMin(row + count, _store->size()) - row;
            beginRemoveRows(QModelIndex(), row, row + count - 1);
            while (count-- > 0) {
                _store->remove(row);
            }
            endRemoveRows();
            return true;
        }

        void TagsModel::abort() {
            emit abortAllJobs();
        }

        void TagsModel::addPath(const QString & path) {
            addPaths({path});
        }

        void TagsModel::addPaths(const QStringList & paths) {
            QStringList copy(paths);

            for (int i = copy.size() - 1; i >= 0; i--) {
                const QString path = copy[i];
                for (int j = 0; j < _directories.size(); j++) {
                    const QString test = _directories[j];
                    if (path == test) {
                        copy.removeAt(i);
                    } else if (test.startsWith(path + '/')) {
                        removeDirectory(test);
                    }
                }
            }

            _directories << copy;

            auto crawler = new Crawler::Crawler(this);

            connect(this, SIGNAL(abortAllJobs()), crawler, SLOT(abort()));

            connect(crawler, SIGNAL(started()), _progress, SIGNAL(started()));
            connect(crawler, SIGNAL(finished()), _progress, SIGNAL(finished()));
            connect(crawler, SIGNAL(progress(int)), _progress, SIGNAL(progress(int)));
            connect(crawler, SIGNAL(rangeChanged(int, int)), _progress, SIGNAL(rangeChanged(int, int)));
            connect(crawler, SIGNAL(finished()), crawler, SLOT(deleteLater()));

            crawler->connect(crawler, &Crawler::Crawler::results, [this](const QList<QVariantHash> & results) {
                beginInsertRows(QModelIndex(), _store->size(), _store->size() + results.size() - 1);
                _store->add(results);
                endInsertRows();
            });

            crawler->setRecursive(_recursive);
            crawler->searchPaths(copy);
        }

        void TagsModel::discardChanges() {

        }

        void TagsModel::reload() {
            const QStringList dirs = _directories;
            _directories.clear();

            removeRows(0, rowCount());
            addPaths(dirs);
        }

        void TagsModel::removeDirectory(const QString & path) {
            for (int i = rowCount() - 1; i >= 0; i--) {
                if (containedDirectoryForRow(i) == path) {
                    removeRow(i);
                }
            }
            _directories.removeOne(path);
        }

        void TagsModel::revert() {
            qDebug() << "revert";

            beginResetModel();
            _store->reset();
            endResetModel();
        }

        QString TagsModel::containedDirectoryForRow(int row) const {
            QDir dir = QFileInfo(container(row).path()).absoluteDir();
            do {
                if (_directories.contains(dir.absolutePath())) {
                    return dir.absolutePath();
                }
            } while (dir.cdUp());

            return QString();
        }

        const Container TagsModel::container(int row) const {
            return container(index(row, 0));
        }

        const Container TagsModel::container(const QModelIndex & idx) const {
            if (idx.isValid()) {
                return _store->at(idx.row());
            }
            return Container();
        }

        bool TagsModel::isRowChanged(const QModelIndex & idx) const {
            return _store->isModified(idx.row());
        }

        void TagsModel::rowChanged(const QModelIndex & idx) {
            emit dataChanged(idx.sibling(idx.row(), 0), idx.sibling(idx.row(), columnCount() - 1));
        }
    }
}
