#ifndef COQUILLO_TAGEDITOR_IMAGEMODEL_H
#define COQUILLO_TAGEDITOR_IMAGEMODEL_H

#include <QAbstractTableModel>
#include <QPointer>

class QNetworkAccessManager;

namespace Coquillo {
    namespace Tags {
        class Container;
        class Image;
    }

    namespace TagEditor {
        class ImageModel : public QAbstractTableModel {
            Q_OBJECT

            public:
                ImageModel(QObject * parent = 0);
                void addRawImage(const QImage & image, const QUrl & path);

                Tags::Image image(int pos) const;

                QVariant data(const QModelIndex & idx, int role=Qt::DisplayRole) const;
                bool dropMimeData(const QMimeData * data, Qt::DropAction action, int row, int col, const QModelIndex & p);
                Qt::ItemFlags flags(const QModelIndex & idx) const;
                QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
                QStringList mimeTypes() const;
                bool removeRows(int row, int count, const QModelIndex & p=QModelIndex());
                int rowCount(const QModelIndex & p = QModelIndex()) const;
                bool setData(const QModelIndex & idx, const QVariant & value, int role = Qt::EditRole);

                inline void setSourceModel(QAbstractItemModel * model) { _source = model; }
                inline QAbstractItemModel * sourceModel() const { return _source; }
                inline int columnCount(const QModelIndex & = QModelIndex()) const { return 4; }
                inline QModelIndex sourceIndex() const { return _index; }

            public slots:
                void setSourceIndex(const QModelIndex & idx);

            private:
                Tags::Container container() const;
                // void setImages(const QList<Tags::Image> & images);
                void addImage(const Tags::Image & image);
                // Tags::Image imageFromPath(const QImage & source, const QUrl & path) const;


                void download(const QUrl & url);
                void initializeImages(const QPersistentModelIndex & idx);

                QPointer<QAbstractItemModel> _source;
                QPersistentModelIndex _index;
                QNetworkAccessManager * _nam;
        };
    }
}

#endif
