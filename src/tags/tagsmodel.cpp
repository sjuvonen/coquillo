#include <QDir>
#include "crawler/crawler.hpp"
#include "../progresslistener.hpp"
#include "tag.hpp"
#include "tagdataroles.hpp"
#include "tagsmodel.hpp"
#include "tagwriter.hpp"

#include <QDebug>

namespace Coquillo {
    namespace Tags {
        TagsModel::TagsModel(ProgressListener * progress, QObject * parent)
        : QAbstractItemModel(parent), _progress(progress), _recursive(false) {
            _columns = QHash<int, QString>();
            _columns[0] = tr("Title");
            _columns[1] = tr("Artist");
            _columns[2] = tr("Album");
            _columns[3] = tr("Genre");
            _columns[4] = tr("Comment");
            _columns[5] = tr("Year");
            _columns[6] = tr("#");
            _columns[7] = tr("Total");
            _columns[8] = tr("Disc");
            _columns[9] = tr("Original Artist");
            _columns[10] = tr("Album Artist");
            _columns[11] = tr("Composer");
            _columns[12] = tr("Url");
            _columns[13] = tr("Encoder");
            _columns[14] = tr("Filename");
            _columns[15] = tr("Images");

            _columnMap = QHash<int, QString>();
            _columnMap[0] = "title";
            _columnMap[1] = "artist";
            _columnMap[2] = "album";
            _columnMap[3] = "genre";
            _columnMap[4] = "comment";
            _columnMap[5] = "year";
            _columnMap[6] = "number";
            _columnMap[7] = "total";
            _columnMap[8] = "disc";
            _columnMap[9] = "original_artist";
            _columnMap[10] = "album_artist";
            _columnMap[11] = "composer";
            _columnMap[12] = "url";
            _columnMap[13] = "encoder";
            _columnMap[14] = "filename";
            _columnMap[15] = "images";

            qRegisterMetaType<Coquillo::Tags::Container>("TagContainer");
        }

        int TagsModel::columnCount(const QModelIndex & parent) const {
            return parent.isValid() ? 0 : _columns.size();
        }

        int TagsModel::rowCount(const QModelIndex & parent) const {
            return parent.isValid() ? 0 : _store.size();
        }

        QVariant TagsModel::data(const QModelIndex & idx, int role) const {
            if (idx.isValid()) {
                if (role == Qt::EditRole || role == Qt::DisplayRole) {
                    const auto file = _store.at(idx.row());
                    if (idx.column() == PathField) {
                        if (role == Qt::EditRole) {
                            return file.path();
                        } else {
                            const auto root = containedDirectoryForRow(idx.row());
                            return file.path().midRef(root.length() + 1).toString();
                        }
                    }
                    const auto field = _columnMap.value(idx.column());
                    return file.value(field);
                } else if (role == ContainerRole) {
                    return QVariant::fromValue(_store.at(idx.row()));
                } else if (role == ItemModifiedStateRole) {
                    return _store.isModified(idx.row());
                } else if (role == FieldModifiedStateRole) {
                    const QString field = _columnMap[idx.column()];
                    return _store.isFieldModified(idx.row(), field);
                } else if (role == RootPathRole) {
                    return containedDirectoryForRow(idx.row());
                }
            }

            return QVariant();
        }

        bool TagsModel::setData(const QModelIndex & idx, const QVariant & value, int role) {
            if (!idx.isValid()) {
                return false;
            }

            if (role != Qt::EditRole && role != Qt::DisplayRole) {
                return QAbstractItemModel::setData(idx, value, role);
            }

            if (idx.column() == PathField) {
                if (_store.rename(idx.row(), value.toString())) {
                    rowChanged(idx);
                    return true;
                }
            } else {
                const QString field = _columnMap[idx.column()];
                if (_store.setValue(idx.row(), field, value)) {
                    rowChanged(idx);
                    return true;
                }
            }

            return false;
        }

        QVariant TagsModel::headerData(int section, Qt::Orientation orientation, int role) const {
            if (orientation == Qt::Horizontal && (role == Qt::EditRole || role == Qt::DisplayRole)) {
                return _columns.value(section);
            }
            return QAbstractItemModel::headerData(section, orientation, role);
        }

        QModelIndex TagsModel::index(int row, int col, const QModelIndex & parent) const {
            if (row >= 0 && row < rowCount() && col >= 0 && col < columnCount() && !parent.isValid()) {
                return createIndex(row, col, qHash(_store.at(row).id(), col));
            } else {
                return QModelIndex();
            }
        }

        bool TagsModel::removeRows(int row, int count, const QModelIndex & parent) {
            if (row < 0 || row >= rowCount()) {
                return false;
            }

            count = qMin(row + count, _store.size()) - row;
            beginRemoveRows(parent, row, row + count - 1);
            while (count-- > 0) {
                _store.remove(row);
            }
            endRemoveRows();
            return true;
        }

        void TagsModel::addPath(const QString & path) {
            addPaths({path});
        }

        void TagsModel::addPaths(const QStringList & paths) {
            QStringList copy(paths);

            for (int i = copy.size() - 1; i >= 0; i--) {
                const QString path = copy[i];
                for (int j = 0; j < _directories.size(); i++) {
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
            crawler->setRecursive(_recursive);
            crawler->searchPaths(copy);

            // connect(crawler, SIGNAL(started()), _progress, SIGNAL(started()));
            // connect(crawler, SIGNAL(finished()), _progress, SIGNAL(finished()));
            // connect(crawler, SIGNAL(progress(int)), _progress, SIGNAL(progress(int)));

            crawler->connect(crawler, &Crawler::Crawler::finished, [this, crawler]{
                crawler->deleteLater();
            });

            crawler->connect(crawler, &Crawler::Crawler::results, [this](const QList<QVariantHash> & results) {
                beginInsertRows(QModelIndex(), _store.size(), _store.size() + results.size() - 1);
                _store.add(results);
                endInsertRows();
            });
        }

        void TagsModel::discardChanges() {

        }

        void TagsModel::reload() {

        }

        void TagsModel::removeDirectory(const QString & path) {
            for (int i = rowCount() - 1; i >= 0; i--) {
                if (containedDirectoryForRow(i) == path) {
                    removeRow(i);
                }
            }
            _directories.removeOne(path);
        }

        void TagsModel::writeToDisk() {
            const QList<Container> items = _store.changedItems();

            if (items.size() > 0) {
                auto * writer = new Writer(this);

                connect(writer, SIGNAL(started()), _progress, SIGNAL(started()));
                connect(writer, SIGNAL(progress(int)), _progress, SIGNAL(progress(int)));
                connect(writer, SIGNAL(finished()), _progress, SIGNAL(finished()));
                connect(writer, SIGNAL(finished()), writer, SLOT(deleteLater()));

                writer->write(items);
            }
        }

        QString TagsModel::containedDirectoryForRow(int row) const {
            QDir dir = QFileInfo(tagContainer(row).path()).absoluteDir();
            do {
                if (_directories.contains(dir.absolutePath())) {
                    return dir.absolutePath();
                }
            } while (dir.cdUp());

            return QString();
        }

        const Container TagsModel::tagContainer(int row) const {
            return tagContainer(index(row, 0));
        }

        const Container TagsModel::tagContainer(const QModelIndex & idx) const {
            if (idx.isValid()) {
                return _store.at(idx.row());
            }
            return Container();
        }

        void TagsModel::rowChanged(const QModelIndex & idx) {
            emit dataChanged(idx.sibling(idx.row(), 0), idx.sibling(idx.row(), columnCount() - 1));
        }
    }
}
