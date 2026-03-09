#include "modificationindicatordelegate.h"
#include "mediastoragemodelroles.h"
#include <qguiapplication.h>
#include <qpalette.h>
#include <qstyleditemdelegate.h>

namespace Coquillo {
ModificationIndicatorDelegate::ModificationIndicatorDelegate(QObject *parent)
    : QStyledItemDelegate(parent) {}

void ModificationIndicatorDelegate::setEditorData(QWidget *editor, const QModelIndex &idx) const {
    if (!editor || !editor->inherits("QLabel")) {
        return;
    }

    if (idx.data(MediaStorageModelRoles::ModificationRole).toBool()) {
        auto palette = QGuiApplication::palette();
        palette.setColor(QPalette::WindowText, palette.accent().color());

        editor->setPalette(palette);
    } else {
        editor->setPalette(QGuiApplication::palette());
    }
}
} // namespace Coquillo