
#include <QAction>
#include <QEvent>
#include <QWidget>
#include "togglewidgetbyaction.hpp"

namespace Coquillo {
    ToggleWidgetByAction::ToggleWidgetByAction(QWidget * widget, QAction * action)
    : QObject(widget), _widget(widget), _action(action) {

        connect(_action, SIGNAL(triggered(bool)), _widget, SLOT(setVisible(bool)));
        _widget->installEventFilter(this);
    }

    bool ToggleWidgetByAction::eventFilter(QObject * watched, QEvent * event) {
        Q_UNUSED(watched)

        if (watched == _widget) {
            if (event->type() == QEvent::Hide) {
                _action->setChecked(false);
            } else if (event->type() == QEvent::Show) {
                _action->setChecked(true);
            }
        }

        return false;
    }
}
