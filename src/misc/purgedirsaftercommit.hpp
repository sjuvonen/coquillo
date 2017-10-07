#ifndef COQUILLO_MISC_PURGEDIRSAFTERCOMMIT_H
#define COQUILLO_MISC_PURGEDIRSAFTERCOMMIT_H

#include <QObject>
#include <QStringList>

namespace Coquillo {
    class PurgeDirsAfterCommit : public QObject {
        Q_OBJECT

        public:
            PurgeDirsAfterCommit(const QStringList & watched, QObject * parent = nullptr);
            void purge();

        private:
            QStringList _dirs;
    };
}

#endif
