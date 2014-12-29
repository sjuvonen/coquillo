
#include <QDebug>
#include <QVariantMap>
#include <taglib/attachedpictureframe.h>
#include <taglib/id3v2tag.h>
#include <taglib/textidentificationframe.h>
#include <taglib/urllinkframe.h>
#include "metadata/mapper.h"
#include "id3v2.h"

#define T2QString(str) QString::fromUtf8((str).toCString(true))

namespace Coquillo {
    namespace MetaData {
        namespace Container {
            Id3v2::Id3v2(TagLib::ID3v2::Tag * tag)
            : Default(tag) {

            }

            QVariantMap Id3v2::read() const {
                QVariantMap data = Default::read();
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

            void Id3v2::write(const QVariantMap & orig) {
                qDebug() << "write id3v2" << orig;

                QVariantMap data(orig);
                const MetaData::Id3v2Mapper mapper;
                const QVariantMap common = {
                    {"album", mapper.take(data, "album")},
                    {"artist", mapper.take(data, "artist")},
                    {"comment", mapper.take(data, "comment")},
                    {"title", mapper.take(data, "title")},
                    {"year", mapper.take(data, "year")},
                };

                Default::write(common);

                auto tag = dynamic_cast<TagLib::ID3v2::Tag*>(_tag);
                const QStringList text_frames = {"TCOM", "TENC", "TOPE", "TPOS"};
                const QStringList supported = (QStringList){"WXXX"} + text_frames;

                foreach (const QString & name, supported) {
                    if (data.contains(name)) {
                        tag->removeFrames(name.toUtf8().constData());
                    }
                }

                if (data.contains("WXXX")) {
                    foreach (const QVariant & url, data.value("WXXX").toList()) {
                        auto frame = new TagLib::ID3v2::UserUrlLinkFrame;
                        frame->setText(url.toString().toStdString());
                        tag->addFrame(frame);
                    }
                }

                foreach (const QString & name, text_frames) {
                    if (data.contains(name)) {
                        foreach (const QVariant & item, data[name].toList()) {
                            auto frame = new TagLib::ID3v2::TextIdentificationFrame(name.toUtf8().data());
                            frame->setText(item.toString().toStdString());
                            tag->addFrame(frame);
                        }
                    }
                }

                if (data.contains("TRCK")) {
                    auto frame = new TagLib::ID3v2::TextIdentificationFrame("TRCK");
                    frame->setText(data["TRCK"].toList().first().toString().toStdString());
                    tag->removeFrames("TRCK");
                    tag->addFrame(frame);
                }
            }
        }
    }
}
