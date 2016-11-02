
#include <taglib/attachedpictureframe.h>
#include <taglib/id3v1genres.h>
#include <taglib/id3v2tag.h>
#include <taglib/textidentificationframe.h>
#include <taglib/urllinkframe.h>
#include "id3v2.hpp"

#include <QDebug>

#define T2QString(str) QString::fromUtf8((str).toCString(true))

namespace Coquillo {
    namespace Crawler {
        namespace Tag {
            QVariantHash Id3v2::read(const TagLib::ID3v2::Tag * tag) const {
                QVariantHash data;
                const auto frames = tag->frameListMap();
                const QStringList ignored = {"APIC"};

                for (auto i = frames.begin(); i != frames.end(); i++) {
                    const QString field = i->first.data();

                    if (!ignored.contains(field)) {
                        for (auto j = i->second.begin(); j != i->second.end();j++) {
                            if (field == "WXXX") {
                                const QRegExp prefix("^\\[\\] ");
                                data.insertMulti(field, T2QString((*j)->toString()).remove(prefix));
                            } else {
                                data.insertMulti(field, T2QString((*j)->toString()));
                            }
                        }
                    }
                }

                /*
                 * Convert legacy genre enum into a string.
                 */
                if (data.contains("TCON")) {
                    bool ok = false;
                    int genre_id = data["TCON"].toInt(&ok);

                    if (ok) {
                        data["TCON"] = T2QString(TagLib::ID3v1::genre(genre_id));
                    }
                }

                if (data.contains("TYER")) {
                    data["TDRC"] = data.take("TYER");
                }

                return data;
            }

            void Id3v2::write(TagLib::ID3v2::Tag * tag, const QVariantHash & values) {
                const QHash<QString, QString> common_map = {
                    {"album", "TALB"},
                    {"artist", "TPE1"},
                    {"comment", "COMM"},
                    {"genre", "TCON"},
                    {"number", "TRCK"},
                    {"title", "TIT2"},
                    {"year", "TDRC"},
                };

                QVariantHash common;

                for (auto i = common_map.constBegin(); i != common_map.constEnd(); i++) {
                    if (values.contains(i.value())) {
                        common.insert(i.key(), values[i.value()]);
                    }
                }

                Generic::write(tag, common);

                const QStringList text_frames = {"TCOM", "TENC", "TOPE", "TPE2", "TPOS"};
                const QStringList supported = (QStringList){"WXXX"} + text_frames;

                foreach (const QString & key, supported) {
                    if (values.contains(key)) {
                        tag->removeFrames(key.toUtf8().constData());
                    }
                }

                foreach (const QString & key, text_frames) {
                    if (values.contains(key)) {
                        foreach (const QVariant & item, values.values(key)) {
                            auto frame = new TagLib::ID3v2::TextIdentificationFrame(key.toUtf8().data());
                            frame->setText(item.toString().toStdString());
                            tag->addFrame(frame);
                        }
                    }
                }

                // if (values.contains("TRCK")) {
                //     auto frame = new TagLib::ID3v2::TextIdentificationFrame("TRCK");
                //     frame->setText(values["TRCK"].toString().toStdString());
                //     tag->removeFrames("TRCK");
                //     tag->addFrame(frame);
                // }

                if (values.contains("WXXX")) {
                    foreach (const QVariant & url, values.values("XWWW")) {
                        auto frame = new TagLib::ID3v2::UserUrlLinkFrame;
                        frame->setText(url.toString().toStdString());
                        tag->addFrame(frame);
                    }
                }
            }
        }
    }
}
