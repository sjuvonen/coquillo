#ifndef COQUILLO_MEDIASTORAGEMODELROLES_H
#define COQUILLO_MEDIASTORAGEMODELROLES_H

#include <qnamespace.h>

namespace Coquillo {
enum MediaStorageModelRoles {
    /**
     * Original absolute path to file.
     */
    FilePathRole = Qt::UserRole + 1
};
}
#endif