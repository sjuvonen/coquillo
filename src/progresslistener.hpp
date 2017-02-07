#ifndef COQUILLO_PROGRESSLISTENER_H
#define COQUILLO_PROGRESSLISTENER_H

#include <QObject>

namespace Coquillo {
    class ProgressListener : public QObject {
        Q_OBJECT

        public:
            ProgressListener(QObject * parent = 0);

        signals:
            void aborted();
            void started();
            void finished();
            void progress(int);
            void rangeChanged(int start, int end);
    };
}

#endif
