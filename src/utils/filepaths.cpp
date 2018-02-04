
#include <QDir>
#include <QRegExp>
#include "filepaths.hpp"

namespace Coquillo {
    namespace Utils {
        namespace FilePaths {
            QString safeFilePath(const QString & path) {
                const QRegExp filter("[^\\/a-z0-9 \\.\\-_\\$&\\(\\)\\[\\]]", Qt::CaseInsensitive);
                return QDir::toNativeSeparators(QDir::fromNativeSeparators(path).replace(filter, ""));
            }

            QString safeFileName(const QString & name) {
                // NOTE: Same filter as in safeFilePath except for forward slash.
                const QRegExp filter("[^a-z0-9 \\.\\-_\\$&\\(\\)\\[\\]]", Qt::CaseInsensitive);
                return QDir::toNativeSeparators(QDir::fromNativeSeparators(name).replace(filter, ""));
            }

            QString mergeFileNames(const QString & source, const QString & target) {
                const QString path = QFileInfo(source).dir().absoluteFilePath(target);
                return QDir::cleanPath(path);
            }
        }
    }
}
