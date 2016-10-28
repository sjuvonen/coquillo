#include "tags/tagdataroles.hpp"
#include "metadatachangeindicatordelegate.hpp"

namespace Coquillo {
    MetaDataChangeIndicatorDelegate::MetaDataChangeIndicatorDelegate(QObject * parent)
        : QStyledItemDelegate(parent) {

    }

    void MetaDataChangeIndicatorDelegate::setEditorData(QWidget * editor, const QModelIndex & idx) const {
        if (!editor || !editor->inherits("QLabel")) {
            return;
        }

        QFont font = editor->font();
        font.setBold(idx.data(Tags::FieldModifiedStateRole).toBool());
        editor->setFont(font);
    }
}
