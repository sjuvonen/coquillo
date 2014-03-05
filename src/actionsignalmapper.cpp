
#include <QAction>
#include "actionsignalmapper.h"

namespace Coquillo {
    void ActionSignalMapper::map(QAction * action) {
        QSignalMapper::map(static_cast<QObject*>(action));
    }
}
