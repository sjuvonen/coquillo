#ifndef COQUILLO_SEARCHER_MUSICBRAINZ_H
#define COQUILLO_SEARCHER_MUSICBRAINZ_H

#include <string>
#include <musicbrainz5/NameCreditList.h>
#include <musicbrainz5/ReleaseList.h>
#include "abstractsearcher.hpp"

namespace MusicBrainz5 {
    class CQuery;
}

namespace Coquillo {
    namespace MetaData {
        class MetaData;
    }

    namespace Searcher {
        class MusicBrainz : public AbstractSearcher {
            Q_OBJECT

            public:
                MusicBrainz(QObject * parent = 0);
                // QString discId(const QList<Coquillo::MetaData::MetaData> & data) const;
                QVariantMap albumInfo(const QString & id, int disc = 0);
                QList<QVariantMap> search(const QVariantMap & params);

            private:
                QString parseNameCredits(const MusicBrainz5::CNameCreditList * names) const;
                MusicBrainz5::CQuery createQuery();
                std::string paramsToQuery(QVariantMap params) const;
                QList<QVariantMap> parseReleases(const MusicBrainz5::CReleaseList *) const;
                const QString _anon;
        };
    }
}

#endif
