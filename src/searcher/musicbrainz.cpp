
#include <QCryptographicHash>

#include <metadata/metadata.h>
#include "musicbrainz.h"

namespace Coquillo {
    namespace Searcher {
        MusicBrainz::MusicBrainz(QObject * parent)
        : AbstractSearcher(parent) {

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

        void MusicBrainz::searchId(const QString & disc_id) {

        }
    }
}
