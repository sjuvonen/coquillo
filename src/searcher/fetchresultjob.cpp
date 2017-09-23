
#include "fetchresultjob.hpp"
#include "musicbrainz.hpp"

namespace Coquillo {
    namespace Searcher {
        FetchResultJob::FetchResultJob(const QString & id, int disc, QObject * parent)
        : QObject(parent), QRunnable(), _id(id), _disc(disc) {
            setAutoDelete(false);
        }

        void FetchResultJob::run() {
            _result = MusicBrainz().albumInfo(_id, _disc);
            emit finished();
        }
    }
}
