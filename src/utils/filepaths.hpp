#ifndef COQUILLO_UTILS_FILEPATHS_H
#define COQUILLO_UTILS_FILEPATHS_H

#include <QString>

namespace Coquillo {
    namespace Utils {
        namespace FilePaths {
            QString safeFilePath(const QString & path);
            QString safeFileName(const QString & name);
            QString mergeFileNames(const QString & source, const QString & target);
        };
    }
}

#endif
