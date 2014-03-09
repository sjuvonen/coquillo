#ifndef COQUILLO_METADATA_MEDIACRAWLER_H
#define COQUILLO_METADATA_MEDIACRAWLER_H

#include <QObject>
#include <QStringList>

namespace Coquillo {
    namespace MetaData {
        class MediaCrawler : public QObject {
            Q_OBJECT

            public:
                MediaCrawler(QObject * parent = 0);

                inline void setNameFilters(const QStringList & filters) { _filters = filters; }
                inline QStringList nameFilters() const { return _filters; }
                inline bool isActive() const { return _active; }
                inline void setDirectory(const QString & dir) { _directory = dir; }
                inline QString directory() const { return _directory; }
                inline void setRecursive(bool state) { _recursive = state; }
                inline bool isRecursive() const { return _recursive; }

            public slots:
                void abort();
                void crawl(const QString & path);
                void start();

            signals:
                void aborted();
                void finished(const QStringList & files);
                void started();

            private:
                QStringList scanDirectory(const QString & path);

                bool _active;
                bool _recursive;
                QString _directory;
                QStringList _filters;
        };
    }
}

#endif
