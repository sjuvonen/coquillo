#ifndef COQUILLO_SEARCHER_ABSTRACTSEARCHER_H
#define COQUILLO_SEARCHER_ABSTRACTSEARCHER_H

#include <QObject>

namespace Coquillo {
    namespace Searcher {
        class AbstractSearcher : public QObject {
            Q_OBJECT

            public:
                AbstractSearcher(QObject * parent = 0);
                virtual ~AbstractSearcher() { }
        };
    }
}

#endif
