#ifndef COQUILLO_TAGEDITOR_TAGEDITOR_H
#define COQUILLO_TAGEDITOR_TAGEDITOR_H

#include <QItemSelectionModel>
#include <QPointer>
#include <QTabWidget>

class QAbstractItemModel;
class QModelIndex;

namespace Coquillo {
    namespace TagEditor {
        class BasicTags;
        class ImageTags;
        class RawData;

        class TagEditor : public QTabWidget {
            Q_OBJECT

            public:
                TagEditor(QWidget * parent = 0);
                void setModel(QAbstractItemModel * model);
                QAbstractItemModel * model() const;
                void setSelectionModel(QItemSelectionModel * model);
                QItemSelectionModel * selectionModel() const { return _selmodel; }

            public slots:
                void setEditorIndex(const QModelIndex & idx);

            private slots:
                void onCloneField(int column, const QVariant & field);

            private:
                BasicTags * _basic;
                ImageTags * _images;
                RawData * _raw;
                QPointer<QItemSelectionModel> _selmodel;
        };
    }
}

#endif
