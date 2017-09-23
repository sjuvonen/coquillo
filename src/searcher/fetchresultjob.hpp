#ifndef COQUILLO_SEARCHER_FETCHRESULTJOB_H
#define COQUILLO_SEARCHER_FETCHRESULTJOB_H

#include <QObject>
#include <QRunnable>
#include <QVariantMap>

namespace Coquillo {
    namespace Searcher {
        class FetchResultJob : public QObject, public QRunnable {
            Q_OBJECT

            public:
                FetchResultJob(const QString & id, int disc = 1, QObject * parent = nullptr);
                void run();

                inline QVariantMap result() const { return _result; }

            signals:
                void finished();

            private:
                QVariantMap _result;
                QString _id;
                int _disc;
        };
    }
}

#endif
