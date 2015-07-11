#include "abstractsearcher.hpp"

namespace Coquillo {
    namespace Searcher {
        AbstractSearcher::AbstractSearcher(const QString & id, QObject * parent)
        : QObject(parent), _id(id) {

        }

        void AbstractSearcher::emitSearchFinished(const QList<QVariantMap> & results) {
            emit searchReady(results, id());
        }
    }
}
