#ifndef COQUILLO_CRAWLER_CRAWLERMANAGER_H
#define COQUILLO_CRAWLER_CRAWLERMANAGER_H

#include <QObject>
#include <QVariantHash>

namespace TagLib {
    class File;
}

namespace Coquillo {
    namespace Crawler {
        class Crawler : public QObject {
            Q_OBJECT

            public:
                Crawler(QObject * parent = 0);

                inline void setRecursive(bool state) { _recursive = state; }
                inline bool isRecursive() const { return _recursive; }

            signals:
                void finished();
                void progress(int value);
                void results(const QList<QVariantHash> & results);

            public slots:
                void searchPath(const QString & path);
                void searchPaths(const QStringList & paths);

            private:
                bool _recursive;
        };

        class DirectoryReader {
            public:
                QStringList read(const QString & path, bool recursive = false);
                QStringList read(const QStringList & paths, bool recursive = false);

            private:
                QStringList nameFilters() const;
        };

        class FileReader {
            public:
                QList<QVariantHash> readFiles(const QStringList & paths) const;
                QVariantHash read(const QString & path) const;

            private:
                bool isFlacFile(const TagLib::File * file) const;
                bool isMpegFile(const TagLib::File * file) const;
                bool isVorbisFile(const TagLib::File * file) const;
        };
    }
}

#endif
