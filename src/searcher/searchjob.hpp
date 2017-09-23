#ifndef COQUILLO_SEARCHER_SEARCHJOB_H
#define COQUILLO_SEARCHER_SEARCHJOB_H

#include <QObject>
#include <QRunnable>
#include <QVariantMap>

namespace Coquillo {
    namespace Searcher {
        class SearchJob : public QObject, public QRunnable {
            Q_OBJECT

            public:
                SearchJob(const QVariantMap & values, QObject * parent = nullptr);
                void run();
                inline QList<QVariantMap> result() const { return _result; }

            signals:
                void finished();

            private:
                QVariantMap _values;
                QList<QVariantMap> _result;
        };
    }
}

#endif
