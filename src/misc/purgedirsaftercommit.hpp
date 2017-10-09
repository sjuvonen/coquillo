#ifndef COQUILLO_TAGS_PURGEDIRSAFTERCOMMIT_H
#define COQUILLO_TAGS_PURGEDIRSAFTERCOMMIT_H

#include <QObject>
#include <QStringList>

namespace Coquillo {
    /**
     * Removes directories that are left empty after moving files around.
     */
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
