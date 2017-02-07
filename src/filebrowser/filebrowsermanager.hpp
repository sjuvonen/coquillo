#ifndef COQUILLO_FILEBROWSERMANAGER_H
#define COQUILLO_FILEBROWSERMANAGER_H

#include <QObject>

namespace Coquillo {
    class FileBrowserManager : public QObject {
        Q_OBJECT

        public:
            FileBrowserManager(QObject * parent = 0);
    };
};

#endif
