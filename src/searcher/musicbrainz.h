#ifndef COQUILLO_SEARCHER_MUSICBRAINZ_H
#define COQUILLO_SEARCHER_MUSICBRAINZ_H

#include "abstractsearcher.h"

namespace Coquillo {
    namespace Searcher {
        class MusicBrainz : public AbstractSearcher {
            Q_OBJECT

            public:
                MusicBrainz(QObject * parent = 0);
//                 QString 

            public slots:
                void searchId(const QString & disc_id);
        };
    }
}

#endif
