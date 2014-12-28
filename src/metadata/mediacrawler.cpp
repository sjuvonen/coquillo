
#include <QDir>
#include <QFileInfo>
#include "mediacrawler.h"

namespace Coquillo {
    namespace MetaData {
        MediaCrawler::MediaCrawler(const QString & dir, QObject * parent)
        : QObject(parent), QRunnable(), _directory(dir), _active(false), _recursive(false) {

        }

        void MediaCrawler::abort() {
            _active = false;
            emit aborted();
        }

        void MediaCrawler::run() {
            const QString path = directory();

            if (QFileInfo(path).isDir()) {
                _directory = path;
                _active = true;
                emit started();

                const QStringList files = scanDirectory(path);
                _active = false;
                emit finished(files);
            } else {
                emit aborted();
            }
        }

        QStringList MediaCrawler::scanDirectory(const QString & path) {
            QFileInfoList entries = QDir(path).entryInfoList(_filters, QDir::Files | QDir::Readable);
            QStringList paths;

            foreach (QFileInfo info, entries) {
                paths << info.absoluteFilePath();
            }

            if (_recursive) {
                entries = QDir(path).entryInfoList(QDir::Dirs | QDir::Readable | QDir::NoDotAndDotDot);
                foreach (QFileInfo info, entries) {
                    paths << scanDirectory(info.absoluteFilePath());
                }
            }

            return paths;
        }
    }
}
