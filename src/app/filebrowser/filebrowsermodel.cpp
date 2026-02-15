#include "filebrowsermodel.h"

#include <qabstractitemmodel.h>
#include <qcontainerfwd.h>
#include <qfileinfo.h>
#include <qlogging.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qsortfilterproxymodel.h>
#include <strings.h>

#include <QFileSystemModel>

bool descendantChecked(const QModelIndex &idx) {
    if (idx.data(Qt::CheckStateRole).toInt() == Qt::CheckState::Checked) {
        return true;
    }

    auto model = idx.model();
    auto rows = model->rowCount(idx);

    for (int i = 0; i < rows; i++) {
        if (descendantChecked(model->index(i, idx.column(), idx))) {
            return true;
        }
    }

    return false;
}

bool ancestorChecked(const QModelIndex &idx) {
    for (auto it = idx; it.isValid(); it = it.parent()) {
        if (it.data(Qt::CheckState::Checked).toInt() == Qt::Checked) {
            return true;
        }
    }

    return false;
}

QStringList filterSubdirectories(QStringList &paths) {
    QStringList erased;

    for (auto it = paths.constBegin(); it != paths.constEnd(); it++) {
        for (auto jt = paths.constBegin(); jt != paths.constEnd(); jt++) {
            if (it != jt && it->startsWith(*jt + '/')) {
                erased << *jt;
                jt = paths.erase(jt) - 1;
            }
        }
    }

    return erased;
}

namespace Coquillo {
FileBrowserModel::FileBrowserModel(QObject *parent)
    : QSortFilterProxyModel(parent), inner(new QFileSystemModel) {
    inner->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
    setSourceModel(inner);
}
FileBrowserModel::~FileBrowserModel() {}

void FileBrowserModel::setDirectory(const QString &directory) {
    if (QFileInfo(directory).isDir()) {
        inner->setRootPath(directory);
    }
}

QModelIndex FileBrowserModel::directoryIndex() {
    return mapFromSource(inner->index(inner->rootPath()));
}

void FileBrowserModel::setRecursive(bool recursive) {
    this->recursive = recursive;

    QModelIndexList updates;

    for (const auto &path : checked) {
        updates << mapFromSource(inner->index(path));
    }

    notifyRecursive(updates);
}

int FileBrowserModel::columnCount(const QModelIndex &parent) const {
    if (!rowCount(parent)) {
        return 0;
    } else {
        return 1;
    }
}

bool FileBrowserModel::hasChildren(const QModelIndex &idx) const {
    if (canFetchMore(idx)) {
        const_cast<FileBrowserModel *>(this)->fetchMore(idx);
    }

    return rowCount(idx) > 0;
}

Qt::ItemFlags FileBrowserModel::flags(const QModelIndex &idx) const {
    Qt::ItemFlags f = QSortFilterProxyModel::flags(idx);
    if (idx.column() == 0 && !(recursive && isAncestorChecked(idx))) {
        f |= Qt::ItemIsUserCheckable;
    }
    return f;
}

QVariant FileBrowserModel::data(const QModelIndex &idx, int role) const {
    if (!idx.isValid()) {
        return QVariant();
    }

    if (role == Qt::CheckStateRole) {
        bool contains = checked.contains(idx.data(QFileSystemModel::FilePathRole).toString());

        if (contains) {
            return Qt::Checked;
        } else {
            if (isDescendantChecked(idx) || (recursive && isAncestorChecked(idx))) {
                return Qt::PartiallyChecked;
            } else {
                return Qt::Unchecked;
            }
        }
    }

    return QSortFilterProxyModel::data(idx, role);
}

bool FileBrowserModel::setData(const QModelIndex &idx, const QVariant &value, int role) {
    if (role == Qt::CheckStateRole) {
        const QString path = idx.data(QFileSystemModel::FilePathRole).toString();
        QModelIndexList updates = {idx};

        if (value.toBool()) {
            checked << path;

            if (recursive) {
                const auto erased = filterSubdirectories(checked);

                for (const auto &path : erased) {
                    updates << mapFromSource(inner->index(path, mapToSource(idx).column()));
                }
            }

            emit pathAdded(path);
        } else {
            checked.removeAll(path);

            emit pathRemoved(path);
        }

        notifyRecursive(updates);

        return true;
    }

    return QSortFilterProxyModel::setData(idx, value, role);
}

void FileBrowserModel::notifyRecursive(const QModelIndexList &targets) {
    QModelIndexList notify = targets;

    while (!notify.empty()) {
        const auto target = notify.takeFirst();

        emit dataChanged(target, target, {Qt::CheckStateRole});

        if (rowCount(target)) {
            for (auto child = index(0, target.column(), target); child.isValid();
                 child = index(child.row() + 1, target.column(), target)) {
                notify << child;
            }

            const auto topLeft = index(0, target.column(), target);
            const auto bottomRight = index(rowCount(target) - 1, target.column(), target);

            emit dataChanged(topLeft, bottomRight, {Qt::CheckStateRole});
        }
    }
}

bool FileBrowserModel::isAncestorChecked(const QModelIndex &idx) const {
    QModelIndex tmp(idx);

    while (tmp.parent().isValid()) {
        if (tmp.parent().data(Qt::CheckStateRole).toInt() == Qt::Checked) {
            return true;
        }

        tmp = tmp.parent();
    }

    return false;
}

bool FileBrowserModel::isDescendantChecked(const QModelIndex &idx) const {
    QString path = idx.data(QFileSystemModel::FilePathRole).toString() + QDir::separator();

    foreach (QString p, checked) {
        if (p.startsWith(path)) {
            return true;
        }
    }
    return false;
}

} // namespace Coquillo
