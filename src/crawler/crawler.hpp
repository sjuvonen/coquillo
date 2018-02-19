#ifndef COQUILLO_CRAWLER_CRAWLERMANAGER_H
#define COQUILLO_CRAWLER_CRAWLERMANAGER_H

#include <QObject>
#include <QVariantMap>

namespace TagLib {
    class File;
}

namespace Coquillo {
    namespace Crawler {
        class ResultStash : public QObject {
            Q_OBJECT

            public:
                ResultStash(int batch_size = 10);
                void add(const QVariantMap & result);
                void add(const QList<QVariantMap> & results);

                void setBatchSize(int batch_size);

                ResultStash & operator<<(const QVariantMap & result);

            public slots:
                void flush();

            signals:
                void results(const QList<QVariantMap> & results);

            private:
                int _limit;
                QList<QVariantMap> _results;
        };

        class Crawler : public QObject {
            Q_OBJECT

            public:
                Crawler(QObject * parent = 0);

                inline void setRecursive(bool state) { _recursive = state; }
                inline bool isRecursive() const { return _recursive; }

            signals:
                void aborted();
                void finished();
                void progress(int value);
                void rangeChanged(int, int);
                void results(const QList<QVariantMap> & results);
                void started();

            public slots:
                void abort();
                void searchPath(const QString & path);
                void searchPaths(const QStringList & paths);

            private:
                volatile bool _aborted = false;
                bool _recursive;
                ResultStash _stash;
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
                // QList<QVariantMap> readFiles(const QStringList & paths, bool with_images = false) const;
                QVariantMap read(const QString & path, bool with_images = false) const;

            private:
                bool isFlacFile(const TagLib::File * file) const;
                bool isMpegFile(const TagLib::File * file) const;
                bool isVorbisFile(const TagLib::File * file) const;
        };
    }
}

#endif
