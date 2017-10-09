#include <QDir>
#include <QFile>
#include <QFileInfo>

#include "renamechangedfiles.hpp"

namespace Coquillo {
    namespace Tags {
        void RenameChangedFiles::run(const QMap<QString, Container> & changed) {
            for (auto i = changed.constBegin(); i != changed.constEnd(); i++) {
                const QString new_path = i.value().path();

                if (i.key() != new_path) {
                    QFileInfo(new_path).dir().mkpath(".");
                    QFile(i.key()).rename(new_path);
                }
            }
        }
    }
}
