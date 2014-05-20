#ifndef COQUILLO_SEARCHER_ABSTRACTSEARCHER_H
#define COQUILLO_SEARCHER_ABSTRACTSEARCHER_H

#include <QObject>
#include <QVariantMap>

namespace Coquillo {
    namespace Searcher {
        class AbstractSearcher : public QObject {
            Q_OBJECT

            public:
                AbstractSearcher(QObject * parent = 0);
                virtual ~AbstractSearcher() { }
                virtual void search(const QVariantMap & fields) = 0;

            signals:
                void searchFinished(const QList<QVariantMap> &);
        };
    }
}

#endif
