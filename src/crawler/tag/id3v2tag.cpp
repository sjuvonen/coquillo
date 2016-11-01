
#include <taglib/attachedpictureframe.h>
#include <taglib/id3v1genres.h>
#include <taglib/id3v2tag.h>
#include <taglib/textidentificationframe.h>
#include <taglib/urllinkframe.h>
#include "id3v2tag.hpp"

#include <QDebug>

#define T2QString(str) QString::fromUtf8((str).toCString(true))

namespace Coquillo {
    namespace Crawler {
        namespace Tag {
            QVariantHash Id3v2Tag::read(const TagLib::ID3v2::Tag * tag) const {
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

            void Id3v2Tag::write(TagLib::ID3v2::Tag * tag, const QVariantHash & values) {
                // QVariantHash common = {
                //     {"album", values.value("TALB")},
                //     {"artist", values.value("TPE1")},
                //     {"comment", values.value("COMM")},
                //     {"genre", values.value("TCON")},
                //     {"title", values.value("TIT2")},
                //     {"year", values.value("TDRC")},
                // };
                //
                // // Default::write(common);
                //
                // auto tag = dynamic_cast<TagLib::ID3v2::Tag*>(_tag);
                // const QStringList text_frames = {"TCOM", "TENC", "TOPE", "TPE2", "TPOS"};
                // const QStringList supported = (QStringList){"WXXX"} + text_frames;
                //
                // foreach (const QString & name, supported) {
                //     if (data.contains(name)) {
                //         tag->removeFrames(name.toUtf8().constData());
                //     }
                // }
                //
                // foreach (const QString & name, text_frames) {
                //     if (data.contains(name)) {
                //         foreach (const QVariant & item, data[name]) {
                //             auto frame = new TagLib::ID3v2::TextIdentificationFrame(name.toUtf8().data());
                //             frame->setText(item.toString().toStdString());
                //             tag->addFrame(frame);
                //         }
                //     }
                // }
                //
                // if (data.contains("TRCK")) {
                //     auto frame = new TagLib::ID3v2::TextIdentificationFrame("TRCK");
                //     frame->setText(data["TRCK"].first().toString().toStdString());
                //     tag->removeFrames("TRCK");
                //     tag->addFrame(frame);
                // }
                //
                // if (data.contains("WXXX")) {
                //     foreach (const QVariant & url, data["WXXX"]) {
                //         auto frame = new TagLib::ID3v2::UserUrlLinkFrame;
                //         frame->setText(url.toString().toStdString());
                //         tag->addFrame(frame);
                //     }
                // }
            }
        }
    }
}
