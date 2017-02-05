#ifndef COQUILLO_TAGS_MODELJOBWATCHER_H
#define COQUILLO_TAGS_MODELJOBWATCHER_H

#include <QObject>
#include <QPointer>

namespace Coquillo {
    namespace Crawler {
        class Crawler;
    }

    namespace Tags {
        class Writer;

        class ModelJobWatcher : public QObject {
            public:
                ModelJobWatcher(QObject * parent = nullptr);

                void add(Crawler::Crawler * crawler);
                void add(Writer * writer);

            public slots:
                void abortAll();

            private:
                QList<QPointer<Crawler::Crawler> > _crawlers;
                QList<QPointer<Writer> > _writers;
        };
    }
}

#endif
