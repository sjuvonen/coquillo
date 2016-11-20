#include <QDir>
#include <QFileInfo>
#include "paths.hpp"

namespace Coquillo {
    namespace Processor {
        Paths::Paths() {

        }

        QString Paths::mergeFileNames(const QString & original, const QString & current) const {
            const QFileInfo new_info(current);

            if (new_info.isAbsolute()) {
                return new_info.absoluteFilePath();
            }

            const QString path = QString("%1/%2").arg(QFileInfo(original).dir().absolutePath(), current);
            return QFileInfo(path).absoluteFilePath();
        }
    }
}
