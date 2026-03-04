#ifndef COQUILLO_METADATAMODEL_H
#define COQUILLO_METADATAMODEL_H

#include "mediastorage.h"
#include <qabstractitemmodel.h>
#include <qpointer.h>

namespace Coquillo {
class MetadataModel : public QAbstractItemModel {
    Q_OBJECT

  public:
    MetadataModel(QObject *parent = nullptr);

    void setStorage(MediaStorage *mediaStorage);
    void setRow(int row);
    void setTag(const QString &tag);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex index(int row, int col, const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &idx, const QVariant &value, int role = Qt::EditRole) override;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    QModelIndex parent(const QModelIndex &idx) const override;

  private:
    QPointer<MediaStorage> storage;
    QString mediaTag;
    int mediaRow;
};
} // namespace Coquillo

#endif