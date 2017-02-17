#ifndef COQUILLO_TAGEDITOR_IMAGETAGS_H
#define COQUILLO_TAGEDITOR_IMAGETAGS_H

#include <QPersistentModelIndex>
#include "editorpagebase.hpp"

class QDataWidgetMapper;

namespace Ui {
    class ImageTags;
}

namespace Coquillo {
    namespace TagEditor {
        class ImageModel;

        class ImageTags : public EditorPageBase {
            Q_OBJECT

            public:
                ImageTags(QWidget * parent = 0);
                ~ImageTags();
                int imageCount() const;
                void setModel(QAbstractItemModel * model);

            public slots:
                void setEditorIndex(const QModelIndex & idx);

            private slots:
                void on_buttonExport_clicked();
                void on_buttonAdd_clicked();
                void on_buttonDelete_clicked();
                void on_buttonCloneImages_clicked();
                void onImageSelect(const QModelIndex & idx);

            private:
                inline ImageModel * imageModel() const { return _images; }

                QModelIndex currentIndex() const;
                int currentRow() const;

                Ui::ImageTags * _ui;
                QDataWidgetMapper * _mapper;
                ImageModel * _images;
                QString _last_dir;
        };
    }
}

#endif
