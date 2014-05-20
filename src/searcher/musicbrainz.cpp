
#include <QCryptographicHash>
#include <QDebug>

#include <musicbrainz5/Artist.h>
#include <musicbrainz5/ArtistCredit.h>
#include <musicbrainz5/NameCredit.h>
#include <musicbrainz5/Query.h>
#include <musicbrainz5/Release.h>
#include <metadata/metadata.h>
#include "musicbrainz.h"

namespace Coquillo {
    namespace Searcher {
        MusicBrainz::MusicBrainz(QObject * parent)
        : AbstractSearcher(parent) {

        }

        void MusicBrainz::search(const QVariantMap & search) {
            MusicBrainz5::CQuery::tParamMap params;
            params["query"] = paramsToQuery(search);
            MusicBrainz5::CQuery query("coquillo/2.0");
            MusicBrainz5::CMetadata result = query.Query("release", "", "", params);
            MusicBrainz5::CReleaseList * releases = result.ReleaseList();

            QList<QVariantMap> results = parseRecordings(releases);

            emit searchFinished(results);
        }

        QString MusicBrainz::discId(const QList<Coquillo::MetaData::MetaData> & disc) const {
            // Regular music CDs have 75 FPS
            const int fps = 75;
            quint32 frames = 2 * fps;
            QCryptographicHash hash(QCryptographicHash::Sha1);

            hash.addData(QString::number(1, 16).toUpper().toUtf8());
            hash.addData(QString::number(disc.count(), 16).toUpper().toUtf8());

            for (int i = 0; i < disc.count(); i++) {
                if (i > 99) {
                    // MusicBrainz ID only considers first <=99 tracks
                    break;
                }

                frames += disc[i].properties().value("length").toInt() * 75;
                hash.addData(QString::number(frames, 16).toUpper().toUtf8());
            }

            QString disc_id = hash.result().toBase64();
            disc_id.replace('+', '.').replace('/', '_').replace('=', '-');

            return disc_id;
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

        QList<QVariantMap> MusicBrainz::parseRecordings(const MusicBrainz5::CReleaseList * releases) const {
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
