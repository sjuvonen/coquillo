
#include <QCryptographicHash>
#include <QDebug>

#include <iostream>

#include <musicbrainz5/Artist.h>
#include <musicbrainz5/ArtistCredit.h>
#include <musicbrainz5/Medium.h>
#include <musicbrainz5/NameCredit.h>
#include <musicbrainz5/Query.h>
#include <musicbrainz5/Recording.h>
#include <musicbrainz5/Release.h>
#include <musicbrainz5/Track.h>
#include <metadata/metadata.h>
#include "musicbrainz.h"

namespace Coquillo {
    namespace Searcher {
        MusicBrainz::MusicBrainz(QObject * parent)
        : AbstractSearcher("musicbrainz", parent) {

        }

        MusicBrainz5::CQuery MusicBrainz::createQuery() {
            return MusicBrainz5::CQuery("coquillo/2.0");
        }

        void MusicBrainz::albumInfo(const QString & id, int disc) {
            MusicBrainz5::CQuery::tParamMap params;
            params["inc"] = "recordings artist-credits";
            MusicBrainz5::CQuery query = createQuery();
            MusicBrainz5::CMetadata result = query.Query("release", id.toStdString(), "", params);
            MusicBrainz5::CRelease * release = result.Release();
            QList<QVariantMap> tracks;

            MusicBrainz5::CMedium * medium = release->MediumList()->Item(disc);

            std::cout << result << std::endl;

            if (medium) {
                for (int i = 0; i < medium->TrackList()->NumItems(); i++) {
                    MusicBrainz5::CTrack * track = medium->TrackList()->Item(i);
                    QVariantMap row;
                    row["number"] = track->Position();
                    row["title"] = QString::fromStdString(track->Recording()->Title());
                    row["artist"] = QString::fromStdString(track->Recording()->ArtistCredit()->NameCreditList()->Item(0)->Artist()->Name());
                    tracks << row;
                }
            }

            QVariantMap album;
            album["tracks"] = QVariant::fromValue< QList<QVariantMap> >(tracks);
            album["title"] = QString::fromStdString(release->Title());
            album["artist"] = QString::fromStdString(release->ArtistCredit()->NameCreditList()->Item(0)->Artist()->Name());
            album["disc"] = disc;
            album["source"] = this->id();

            emit albumReady(album);
        }

        void MusicBrainz::search(const QVariantMap & search) {
            MusicBrainz5::CQuery::tParamMap params;
            params["query"] = paramsToQuery(search);
            MusicBrainz5::CQuery query = createQuery();
            MusicBrainz5::CMetadata result = query.Query("release", "", "", params);
            MusicBrainz5::CReleaseList * releases = result.ReleaseList();
            QList<QVariantMap> results = parseReleases(releases);

            emitSearchFinished(results);
        }

        std::string MusicBrainz::paramsToQuery(QVariantMap params) const {
            QStringList query;

            if (params.contains("album")) {
                query.append(QString("\"%1\"").arg(params.take("album").toString()));
            }

            foreach (QString key, params.keys()) {
                query.append(QString("%1:\"%2\"").arg(key, params[key].toString()));
            }

            return query.join(" AND ").toStdString();
        }

        QList<QVariantMap> MusicBrainz::parseReleases(const MusicBrainz5::CReleaseList * releases) const {
            QList<QVariantMap> results;

            if (!releases) {
                return results;
            }

            int count = qMin(releases->Count(), 25);

            for (int i = 0; i < count; i++) {
                QVariantMap data;
                const MusicBrainz5::CRelease * r = releases->Item(i);

                if (r) {
                    data["id"] = QString::fromStdString(r->ID());
                    data["title"] = QString::fromStdString(r->Title());
                    data["disc"] = 1;

                    if (r->ArtistCredit()) {
                        const MusicBrainz5::CNameCreditList * names = r->ArtistCredit()->NameCreditList();

                        if (names->NumItems() > 0) {
                            data["artist"] = QString::fromStdString(names->Item(0)->Artist()->Name());
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
