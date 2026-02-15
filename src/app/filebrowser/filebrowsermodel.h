#ifndef COQUILLO_FILEBROWSERMODEL_H
#define COQUILLO_FILEBROWSERMODEL_H

#include <QFileSystemModel>
#include <QSortFilterProxyModel>
#include <qabstractitemmodel.h>
#include <qobject.h>

namespace Coquillo {

class FileBrowserModel : public QSortFilterProxyModel {
    Q_OBJECT

  public:
    FileBrowserModel(QObject *parent = nullptr);
    ~FileBrowserModel();

    void setDirectory(const QString &directory);
    QModelIndex directoryIndex();

    void setRecursive(bool recursive);

    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    bool setData(const QModelIndex &idx, const QVariant &value, int role = Qt::EditRole);
    bool hasChildren(const QModelIndex &idx) const;

    Qt::ItemFlags flags(const QModelIndex &idx) const;
    QVariant data(const QModelIndex &idx, int role) const;

  signals:
    void pathAdded(const QString &path);
    void pathRemoved(const QString &path);

  private:
    bool isAncestorChecked(const QModelIndex &idx) const;
    bool isDescendantChecked(const QModelIndex &idx) const;
    void notifyRecursive(const QModelIndexList &targets);

    QFileSystemModel *inner;
    QStringList checked;
    bool recursive;
};
} // namespace Coquillo

#endif
