#ifndef COQUILLO_CURRENTMEDIAPATHDELEGATE_H
#define COQUILLO_CURRENTMEDIAPATHDELEGATE_H

#include "selectionnotifier.h"
#include <QComboBox>
#include <QItemSelectionModel>
#include <QObject>
#include <QPointer>

QT_BEGIN_NAMESPACE
class QComboBox;
class QItemSelectionModel;
QT_BEGIN_NAMESPACE

namespace Coquillo {
class CurrentMediaPathDelegate : public QObject {
    Q_OBJECT

  public:
    CurrentMediaPathDelegate(QObject *parent = nullptr);

    void setSelectionNotifier(SelectionNotifier *selectionNotifier);
    void setComboBox(QComboBox *comboBox);

  private:
    QPointer<SelectionNotifier> _selectionNotifier;
    QPointer<QComboBox> _comboBox;
};
} // namespace Coquillo

#endif