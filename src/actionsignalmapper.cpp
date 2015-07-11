
#include <QAction>
#include "actionsignalmapper.hpp"

namespace Coquillo {
    void ActionSignalMapper::map(QAction * action) {
        QSignalMapper::map(static_cast<QObject*>(action));
    }
}
