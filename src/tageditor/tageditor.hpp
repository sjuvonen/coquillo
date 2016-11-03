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
                QAbstractItemModel * model() const;
                void setModel(QAbstractItemModel * model);
                void setSelectionModel(QItemSelectionModel * model);

            public slots:
                void setEditorIndex(const QModelIndex & idx);

            private:
                BasicTags * _tabBasic;
                ImageTags * _tabImages;
                RawData * _tabRaw;
                QPointer<QItemSelectionModel> _selection;
        };
    }
}

#endif
