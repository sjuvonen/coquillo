
#include <taglib/attachedpictureframe.h>
#include <taglib/id3v1genres.h>
#include <taglib/id3v2tag.h>
#include <taglib/textidentificationframe.h>
#include <taglib/urllinkframe.h>
#include <QImage>
#include "id3v2.hpp"

#include <QDebug>

#define T2QString(str) QString::fromUtf8((str).toCString(true))

namespace Coquillo {
    namespace Crawler {
        namespace Tag {
            void Id3v2::write(TagLib::ID3v2::Tag * tag, const TagData & values) {
                const QHash<QString, QString> common_map = {
                    {"album", "TALB"},
                    {"artist", "TPE1"},
                    {"comment", "COMM"},
                    {"genre", "TCON"},
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

                const QStringList text_frames = {"TCOM", "TENC", "TOPE", "TPE2"};
                const QStringList supported = (QStringList){"WXXX"} + text_frames;

                foreach (const QString & key, supported) {
                    if (values.contains(key)) {
                        tag->removeFrames(key.toUtf8().constData());
                    }
                }

                foreach (const QString & key, text_frames) {
                    if (values.contains(key)) {
                        foreach (const QVariant & item, values.values(key)) {
                            auto frame = new TagLib::ID3v2::TextIdentificationFrame(key.toUtf8().constData());
                            frame->setText(item.toString().toStdString());
                            tag->addFrame(frame);
                        }
                    }
                }

                foreach (const QString key, QStringList({"TRCK", "TPOS"})) {
                    if (values.contains(key)) {
                        const QVariant number = values.value(key);
                        const QVariant total = values.value(key + "[b]");
                        auto frame = new TagLib::ID3v2::TextIdentificationFrame(key.toUtf8().constData());

                        tag->removeFrames(key.toUtf8().constData());

                        if (total.isValid()) {
                            const auto value = QString("%1/%2").arg(number.toInt()).arg(total.toInt());
                            frame->setText(value.toStdString());
                        } else {
                            frame->setText(number.toString().toStdString());
                        }
                    }
                }

                if (values.contains("WXXX")) {
                    foreach (const QVariant & url, values.values("XWWW")) {
                        auto frame = new TagLib::ID3v2::UserUrlLinkFrame;
                        frame->setText(url.toString().toStdString());
                        tag->addFrame(frame);
                    }
                }
            }

            TagData Id3v2::read(const TagLib::ID3v2::Tag * tag) const {
                TagData data;
                const auto frames = tag->frameListMap();
                const QStringList ignored = {"APIC"};

                for (auto i = frames.begin(); i != frames.end(); i++) {
                    // Some apps seem to have added unicode trash at the end of tags.
                    const QString field = QString(i->first.data()).mid(0, 4);

                    if (!ignored.contains(field)) {
                        for (auto j = i->second.begin(); j != i->second.end();j++) {
                            if (field == "WXXX") {
                                const QRegExp prefix("^\\[\\] ");
                                data.insertMulti(field, T2QString((*j)->toString()).remove(prefix));
                            } else if (field == "TRCK" || field == "TPOS") {
                                const auto parts = T2QString((*j)->toString()).split('/');
                                data.insertMulti(field, parts.value(0));
                                if (parts.size() > 1) {
                                    data.insertMulti(field + "[b]", parts.value(1));
                                }
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


            ImageDataList Id3v2::readImages(const TagLib::ID3v2::Tag * tag) const {
                const auto frames = tag->frameList("APIC");
                ImageDataList images;

                for (auto i = frames.begin(); i != frames.end(); i++) {
                    const auto frame = static_cast<TagLib::ID3v2::AttachedPictureFrame*>(*i);

                    /*
                     * WARNING: For some reason fromData() fails if bytes are
                     * stored into a temp variable in between
                     */
                    const QImage source = QImage::fromData(reinterpret_cast<uchar*>(frame->picture().data()), frame->picture().size());

                    ImageData image;
                    image["data"] = source.copy();
                    image["description"] = T2QString(frame->description());
                    image["type"] = frame->type();
                    image["mime"] = T2QString(frame->mimeType());

                    images << image;
                }

                // qDebug() << "I" << images.size();
                return images;
            }
        }
    }
}
