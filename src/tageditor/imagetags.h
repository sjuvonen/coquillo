#ifndef COQUILLO_TAGEDITOR_IMAGETAGS_H
#define COQUILLO_TAGEDITOR_IMAGETAGS_H

#include <QPersistentModelIndex>
#include <QWidget>

class QAbstractItemModel;
class QDataWidgetMapper;
class QModelIndex;

namespace Ui {
    class ImageTags;
}

namespace Coquillo {
    namespace MetaData {
        class MetaData;
    }

    namespace TagEditor {
        class ImageModel;

        class ImageTags : public QWidget {
            Q_OBJECT

            public:
                ImageTags(QWidget * parent = 0);
                ~ImageTags();
                int imageCount() const;

            public slots:
                void setEditorIndex(const QModelIndex & idx);

            signals:
                void cloneField(int column, const QVariant & value);

            private slots:
                void on_cloneImages_clicked();
                void onImageSelect(const QModelIndex & idx);

            private:
                MetaData::MetaData metaData(const QModelIndex &) const;
                Ui::ImageTags * _ui;
                QPersistentModelIndex _current;
                QDataWidgetMapper * _mapper;
                ImageModel * _model;
        };
    }
}

#endif
