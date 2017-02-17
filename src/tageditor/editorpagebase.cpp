
#include "editorpagebase.hpp"

namespace Coquillo {
    namespace TagEditor {
        EditorPageBase::EditorPageBase(QWidget * parent)
        : QWidget(parent) {

        }

        void EditorPageBase::setEditorIndex(const QModelIndex & index) {
            _index = index;
        }

        void EditorPageBase::setSelectionModel(QItemSelectionModel * model) {
            _selection = model;
        }

        void EditorPageBase::setModel(QAbstractItemModel * model) {
            _model = model;
        }

        QAbstractItemModel * EditorPageBase::model() const {
            return _model;
        }

        QItemSelectionModel * EditorPageBase::selectionModel() const {
            return _selection;
        }

        QModelIndex EditorPageBase::editorIndex() const {
            return _index;
        }
    }
}
