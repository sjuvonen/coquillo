#include <QDebug>

#include <metadata/metadatamodel.h>
#include "metadatachangeindicatordelegate.h"

namespace Coquillo {
    MetaDataChangeIndicatorDelegate::MetaDataChangeIndicatorDelegate(QObject * parent)
        : QStyledItemDelegate(parent) {

    }

    void MetaDataChangeIndicatorDelegate::setEditorData(QWidget * editor, const QModelIndex & idx) const {
        if (!editor || !editor->inherits("QLabel")) {
            return;
        }

        QFont font = editor->font();
        font.setBold(idx.data(MetaData::MetaDataModel::ModifiedRole).toBool());
        editor->setFont(font);
    }
}
