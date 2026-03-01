#ifndef COQUILLO_MEDIASTORAGEMODEL_H
#define COQUILLO_MEDIASTORAGEMODEL_H

#include "mediastorage.h"
#include <QAbstractItemModel>
#include <QPointer>
#include <qnamespace.h>

namespace Coquillo {
class MediaStorageModel : public QAbstractItemModel {
    Q_OBJECT

  public:
    MediaStorageModel(QObject *parent = nullptr);
    void setStorage(MediaStorage *storage);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex index(int row, int col, const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &idx, const QVariant &value, int role = Qt::EditRole) override;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    inline QModelIndex parent(const QModelIndex &idx) const override;

  protected slots:
    void update();

  private:
    QPointer<MediaStorage> storage;
    int size;
};

QModelIndex MediaStorageModel::parent(const QModelIndex &idx) const { return QModelIndex(); }
} // namespace Coquillo
#endif