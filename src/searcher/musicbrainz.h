#ifndef COQUILLO_SEARCHER_MUSICBRAINZ_H
#define COQUILLO_SEARCHER_MUSICBRAINZ_H

#include <string>
#include <musicbrainz5/ReleaseList.h>
#include "abstractsearcher.h"

namespace Coquillo {
    namespace MetaData {
        class MetaData;
    }

    namespace Searcher {
        class MusicBrainz : public AbstractSearcher {
            Q_OBJECT

            public:
                MusicBrainz(QObject * parent = 0);
                QString discId(const QList<Coquillo::MetaData::MetaData> & data) const;
                void search(const QVariantMap & params);

            private:
                std::string paramsToQuery(QVariantMap params) const;
                QList<QVariantMap> parseRecordings(const MusicBrainz5::CReleaseList *) const;
        };
    }
}

#endif
