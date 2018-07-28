#include <QDir>
#include <QFile>
#include <QFileInfo>

#include "renamechangedfiles.hpp"
#include "utils/filepaths.hpp"

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

        QString RenameChangedFiles::safeFilePath(const QString & path) {
            return Utils::FilePaths::safeFilePath(path);
        }

        QString RenameChangedFiles::safeFileName(const QString & name) {
            return Utils::FilePaths::safeFileName(name);
        }
    }
}
