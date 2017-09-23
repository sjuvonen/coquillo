#ifndef COQUILLO_SEARCHER_ABSTRACTSEARCHER_H
#define COQUILLO_SEARCHER_ABSTRACTSEARCHER_H

#include <QObject>
#include <QVariantMap>

namespace Coquillo {
    namespace Searcher {
        class AbstractSearcher : public QObject {
            Q_OBJECT

            public:
                virtual ~AbstractSearcher() { }
                virtual QList<QVariantMap> search(const QVariantMap & fields) = 0;
                virtual QVariantMap albumInfo(const QString & id, int disc = 0) = 0;
                inline QString id() const { return _id; }

            signals:
                void searchReady(const QList<QVariantMap> & results, const QString & source);
                void albumReady(const QVariantMap & data);

            protected:
                AbstractSearcher(const QString & id, QObject * parent = 0);
                void emitSearchFinished(const QList<QVariantMap> & results);

            private:
                const QString _id;
        };
    }
}

#endif
