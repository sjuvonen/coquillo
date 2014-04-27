#ifndef COQUILLO_SEARCHER_ABSTRACTSEARCHER_H
#define COQUILLO_SEARCHER_ABSTRACTSEARCHER_H

#include <QObject>

namespace Coquillo {
    namespace Searcher {
        typedef QList<QVariantMap> Results;

        class AbstractSearcher : public QObject {
            Q_OBJECT

            public:
                AbstractSearcher(QObject * parent = 0);
                virtual ~AbstractSearcher() { }
                virtual void search(const QVariantMap & fields) = 0;

            signals:
                void searchResults(const Results &);

        };
    }
}

#endif
