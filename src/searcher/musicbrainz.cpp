
#include <QCryptographicHash>
#include <QDebug>

#include <musicbrainz5/Artist.h>
#include <musicbrainz5/ArtistCredit.h>
#include <musicbrainz5/Medium.h>
#include <musicbrainz5/NameCredit.h>
#include <musicbrainz5/Query.h>
#include <musicbrainz5/Recording.h>
#include <musicbrainz5/Release.h>
#include <musicbrainz5/Track.h>
#include "musicbrainz.hpp"

namespace Coquillo {
    namespace Searcher {
        MusicBrainz::MusicBrainz(QObject * parent)
        : AbstractSearcher("musicbrainz", parent), _anon("89ad4ac3-39f7-470e-963a-56509c546377") {

        }

        MusicBrainz5::CQuery MusicBrainz::createQuery() {
            return MusicBrainz5::CQuery("coquillo/2.0");
        }

        QVariantMap MusicBrainz::albumInfo(const QString & id, int disc) {
            MusicBrainz5::CQuery::tParamMap params;
            params["inc"] = "recordings artist-credits";
            MusicBrainz5::CQuery query = createQuery();
            MusicBrainz5::CMetadata result = query.Query("release", id.toStdString(), "", params);
            MusicBrainz5::CRelease * release = result.Release();
            QList<QVariantMap> tracks;

            MusicBrainz5::CMedium * medium = release->MediumList()->Item(disc - 1);

            if (medium) {
                for (int i = 0; i < medium->TrackList()->NumItems(); i++) {
                    MusicBrainz5::CTrack * track = medium->TrackList()->Item(i);
                    const QVariantMap row = {
                        {"number", track->Position()},
                        {"title", QString::fromStdString(track->Recording()->Title())},
                        {"artist", parseNameCredits(track->Recording()->ArtistCredit()->NameCreditList())},
                    };
                    tracks << row;
                }
            }

            QVariantMap album = {
                {"tracks", QVariant::fromValue< QList<QVariantMap> >(tracks)},
                {"title", QString::fromStdString(release->Title())},
                {"artist", parseNameCredits(release->ArtistCredit()->NameCreditList())},
                {"date", QString::fromStdString(release->Date())},
                {"disc", disc},
                {"source", this->id()},
                {"id", id},
                {"x-musicbrainz-id", id},
            };

            return album;
        }

        QList<QVariantMap> MusicBrainz::search(const QVariantMap & search) {
            MusicBrainz5::CQuery::tParamMap params;
            params["query"] = paramsToQuery(search);
            MusicBrainz5::CQuery query = createQuery();
            MusicBrainz5::CMetadata result = query.Query("release", "", "", params);
            MusicBrainz5::CReleaseList * releases = result.ReleaseList();
            QList<QVariantMap> results = parseReleases(releases);
            return results;
        }

        std::string MusicBrainz::paramsToQuery(QVariantMap params) const {
            QStringList query;
            query.append(QString("\"%1\"").arg(params.take("album").toString()));

            foreach (QString key, params.keys()) {
                query.append(QString("%1:\"%2\"").arg(key, params[key].toString()));
            }

            return query.join(" AND ").toStdString();
        }

        QString MusicBrainz::parseNameCredits(const MusicBrainz5::CNameCreditList * names) const {
            QString full;

            for (int i = 0; i < names->NumItems(); i++) {
                MusicBrainz5::CNameCredit * credit  = names->Item(i);
                const QString name = QString::fromStdString(credit->Artist()->Name());
                const QString glue = QString::fromStdString(credit->JoinPhrase());
                full += name + glue;
            }

            return full.trimmed();
        }

        QList<QVariantMap> MusicBrainz::parseReleases(const MusicBrainz5::CReleaseList * releases) const {
            QList<QVariantMap> results;

            if (!releases) {
                return results;
            }

            int count = qMin(releases->Count(), 25);

            for (int i = 0; i < count; i++) {
                const MusicBrainz5::CRelease * r = releases->Item(i);

                if (r) {
                    QVariantMap data = {
                        {"id", QString::fromStdString(r->ID())},
                        {"title", QString::fromStdString(r->Title())},
                        {"disc", 1},
                    };

                    if (r->ArtistCredit()) {
                        const MusicBrainz5::CNameCreditList * names = r->ArtistCredit()->NameCreditList();

                        if (names->NumItems() > 0) {
                            data["artist"] = parseNameCredits(names);
                        } else {
                            data["artist"] = QString("None");
                        }
                    }

                    results.append(data);

                    if (r->MediumList() && r->MediumList()->Count() > 1) {
                        for (int i = 1; i < r->MediumList()->Count(); i++) {
                            data["disc"] = i+1;
                            results.append(data);
                        }
                    }
                }
            }

            return results;
        }
    }
}
