
#include "musicbrainz.hpp"
#include "searchjob.hpp"

#include <QDebug>

namespace Coquillo {
    namespace Searcher {
        SearchJob::SearchJob(const QVariantMap & values, QObject * parent)
        : QObject(parent), QRunnable(), _values(values) {
            // setAutoDelete(false);
        }

        void SearchJob::run() {
            MusicBrainz searcher;
            _result = searcher.search(_values);

            emit finished();
        }
    }
}
