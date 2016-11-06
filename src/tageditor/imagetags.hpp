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
    namespace TagEditor {
        class ImageModel;

        class ImageTags : public QWidget {
            Q_OBJECT

            public:
                ImageTags(QWidget * parent = 0);
                ~ImageTags();
                int imageCount() const;
                void setModel(QAbstractItemModel * model);
                QAbstractItemModel * model() const;

            public slots:
                void setEditorIndex(const QModelIndex & idx);

            signals:
                void cloneField(int column, const QVariant & value);

            private slots:
                void addImage();
                void exportCurrentImage();
                void on_cloneImages_clicked();
                void onImageSelect(const QModelIndex & idx);
                void removeCurrentRow();

            private:
                inline ImageModel * imageModel() const { return _model; }
                Ui::ImageTags * _ui;
                QDataWidgetMapper * _mapper;
                ImageModel * _model;
                QString _last_dir;
        };
    }
}

#endif
