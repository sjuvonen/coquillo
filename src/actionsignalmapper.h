#ifndef COQUILLO_ACTIONSIGNALMAPPER_H
#define COQUILLO_ACTIONSIGNALMAPPER_H

#include <QSignalMapper>

class QAction;

namespace Coquillo {
    class ActionSignalMapper : public QSignalMapper {
        Q_OBJECT

        public:
            ActionSignalMapper(QObject * parent = 0) : QSignalMapper(parent) { }

        public slots:
            void map(QAction * action);
    };
}

#endif
