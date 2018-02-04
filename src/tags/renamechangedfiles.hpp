#ifndef COQUILLO_TAGS_RENAMECHANGEDFILES_H
#define COQUILLO_TAGS_RENAMECHANGEDFILES_H

#include <QMap>
#include <QString>

#include "tags/tagcontainer.hpp"

namespace Coquillo {
    namespace Tags {
        /**
         * Removes directories that are left empty after moving files around.
         */
        class RenameChangedFiles {
            public:
                // PurgeDirsAfterCommit() {};
                void run(const QMap<QString, Container> & changed);

                static QString safeFilePath(const QString & path);
                static QString safeFileName(const QString & name);
        };
    }
}

#endif
