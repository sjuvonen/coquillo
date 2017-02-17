#ifndef COQUILLO_TAGEDITOR_PAGEWIDGETBASE_H
#define COQUILLO_TAGEDITOR_PAGEWIDGETBASE_H

#include <QAbstractItemModel>
#include <QItemSelectionModel>
#include <QPointer>
#include <QWidget>

namespace Coquillo {
    namespace TagEditor {
        class EditorPageBase : public QWidget {
            Q_OBJECT

            public:
                EditorPageBase(QWidget * parent = nullptr);
                virtual ~EditorPageBase() { }

                virtual void setModel(QAbstractItemModel * model);
                QAbstractItemModel * model() const;

                virtual void setSelectionModel(QItemSelectionModel * model);
                QItemSelectionModel * selectionModel() const;

                QModelIndex editorIndex() const;

            public slots:
                virtual void setEditorIndex(const QModelIndex & idx);

            protected:
                QPointer<QAbstractItemModel> _model;
                QPointer<QItemSelectionModel> _selection;
                QPersistentModelIndex _index;
        };
    }
}

#endif
