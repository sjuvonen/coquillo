#ifndef COQUILLO_MEDIASTORAGEMODEL_H
#define COQUILLO_MEDIASTORAGEMODEL_H

#include "mediastorage.h"
#include <QAbstractItemModel>
#include <QPointer>

namespace Coquillo {
class MediaStorageModel : public QAbstractItemModel {
    Q_OBJECT

  public:
    MediaStorageModel(QObject *parent = nullptr);
    void setStorage(MediaStorage *storage);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QModelIndex index(int row, int col, const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    inline QModelIndex parent(const QModelIndex &idx) const { return QModelIndex(); }

  protected slots:
    void update();

  private:
    QPointer<MediaStorage> storage;
    int size;
};
} // namespace Coquillo
#endif