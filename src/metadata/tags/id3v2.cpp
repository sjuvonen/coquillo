
#include <QDebug>
#include <QVariantMap>
#include <taglib/id3v2tag.h>
#include "metadata/mapper.h"
#include "id3v2.h"

#define T2QString(str) QString::fromUtf8((str).toCString(true))

namespace Coquillo {
    namespace MetaData {
        namespace Container {
            Id3v2::Id3v2(TagLib::ID3v2::Tag * tag)
            : AbstractTag(tag) {

            }

            QVariantMap Id3v2::read() const {
                QVariantMap data = readCommon();
                const auto tag = dynamic_cast<TagLib::ID3v2::Tag*>(_tag);
                const auto frames = tag->frameListMap();

                if (tag->frameList("TCOM").size() > 0) {
                    data.insert("composer", T2QString(frames["TCOM"].front()->toString()));
                }

                if (tag->frameList("TENC").size() > 0) {
                    data.insert("encoder", T2QString(frames["TENC"].front()->toString()));
                }

                if (tag->frameList("TOPE").size() > 0) {
                    data.insert("original_artist", T2QString(frames["TOPE"].front()->toString()));
                }

                if (tag->frameList("TPE2").size() > 0) {
                    data.insert("album_artist", T2QString(frames["TPE2"].front()->toString()));
                }

                if (tag->frameList("TPOS").size() > 0) {
                    data.insert("disc", T2QString(frames["TPOS"].front()->toString()).toInt());
                }

                if (tag->frameList("WXXX").size() > 0) {
                    data.insert("url", T2QString(frames["WXXX"].front()->toString()).remove(QRegExp("^\\[\\] ")));
                }

                if (tag->frameList("TRCK").size() > 0) {
                    const QString track = T2QString(frames["TRCK"].front()->toString());
                    const QStringList parts = track.split('/');

                    if (parts.count() >= 2) {
                        data.insert("total", parts[1].toInt());
                    }
                }

                for (auto i = frames.begin(); i != frames.end(); i++) {
                    const QString field = i->first.data();
                    QVariantList values;

                    for (auto j = i->second.begin(); j != i->second.end();j++) {
                        values << T2QString((*j)->toString());
                    }

                    data.insert(field, values);
                }

                return data;
            }

            void Id3v2::write(const QVariantMap & data) {
                qDebug() << "write id3v2" << data;
                
                const MetaData::Id3v2Mapper mapper;
                const QVariantMap common = {
                    {"album", mapper.value(data, "album")},
                    {"artist", mapper.value(data, "artist")},
                    {"comment", mapper.value(data, "comment")},
                    {"number", mapper.value(data, "number")},
                    {"title", mapper.value(data, "title")},
                    {"year", mapper.value(data, "year")},
                };
                writeCommon(common);
            }
        }
    }
}
