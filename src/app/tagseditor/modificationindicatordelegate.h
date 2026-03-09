#ifndef COQUILLO_TAGEDITOR_MODIFICATIONINDICATORDELEGATE_H
#define COQUILLO_TAGEDITOR_MODIFICATIONINDICATORDELEGATE_H

#include <QStyledItemDelegate>

namespace Coquillo {
class ModificationIndicatorDelegate : public QStyledItemDelegate {
    Q_OBJECT

  public:
    ModificationIndicatorDelegate(QObject *parent = nullptr);

    void setEditorData(QWidget *editor, const QModelIndex &idx) const override;
};
} // namespace Coquillo

#endif