#ifndef COQUILLO_TOGGLEWIDGETBYACTION_H
#define COQUILLO_TOGGLEWIDGETBYACTION_H

#include <QObject>
#include <QPointer>

class QAction;
class QWidget;

/**
 * Manage display of a widget based on checkstate of a QAction
 */
namespace Coquillo {
    class ToggleWidgetByAction : public QObject {
        Q_OBJECT

        public:
            ToggleWidgetByAction(QWidget * widget, QAction * action);
            bool eventFilter(QObject * watched, QEvent * event);

        private:
            QPointer<QWidget> _widget;
            QPointer<QAction> _action;
    };
}

#endif
