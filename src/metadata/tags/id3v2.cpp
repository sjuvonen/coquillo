
#include <QDebug>
#include <taglib/attachedpictureframe.h>
#include <taglib/id3v2tag.h>
#include <taglib/textidentificationframe.h>
#include <taglib/urllinkframe.h>
#include "metadata/image.h"
#include "metadata/mapper.h"
#include "id3v2.h"

#define T2QString(str) QString::fromUtf8((str).toCString(true))

namespace Coquillo {
    namespace MetaData {
        namespace Container {
            Id3v2::Id3v2(TagLib::ID3v2::Tag * tag)
            : Default(tag) {

            }

            Tag Id3v2::read() const {
                Tag data;
                const auto tag = dynamic_cast<TagLib::ID3v2::Tag*>(_tag);
                const auto frames = tag->frameListMap();
                const QStringList ignored = {"APIC"};

                for (auto i = frames.begin(); i != frames.end(); i++) {
                    const QString field = i->first.data();

                    if (!ignored.contains(field)) {
                        QVariantList values;

                        for (auto j = i->second.begin(); j != i->second.end();j++) {
                            values << T2QString((*j)->toString());
                        }

                        data.insert(field, values);
                    }
                }

                if (data.contains("TYER")) {
                    data["TDRL"] = data.take("TYER");
                }

                return data;
            }

            ImageList Id3v2::readImages() const {
                auto tag = dynamic_cast<TagLib::ID3v2::Tag*>(_tag);
                const auto frames = tag->frameList("APIC");
                QList<Image> images;
                for (auto i = frames.begin(); i != frames.end(); i++) {
                    const auto frame = static_cast<TagLib::ID3v2::AttachedPictureFrame*>(*i);

                    /*
                     * WARNING: For some reason fromData() fails if bytes are
                     * stored into a temp variable in between
                     */
                    const QImage source = QImage::fromData(reinterpret_cast<uchar*>(frame->picture().data()), frame->picture().size());

                    Image image;
                    image.setSource(source);
                    image.setDescription(T2QString(frame->description()));
                    image.setType(frame->type());
                    image.setMimeType(T2QString(frame->mimeType()));
                    images << image;
                }
                return images;
            }

            void Id3v2::write(const Tag & orig) {
                qDebug() << "write id3v2" << orig;

                Tag data(orig);
                const Id3v2Mapper mapper;
                const Tag common = {
                    {"album", {mapper.take(data, "album")}},
                    {"artist", {mapper.take(data, "artist")}},
                    {"comment", {mapper.take(data, "comment")}},
                    {"genre", {mapper.take(data, "genre")}},
                    {"title",{mapper.take(data, "title")}},
                    {"year", {mapper.take(data, "year")}},
                };

                Default::write(common);

                auto tag = dynamic_cast<TagLib::ID3v2::Tag*>(_tag);
                const QStringList text_frames = {"TCOM", "TENC", "TOPE", "TPE2", "TPOS"};
                const QStringList supported = (QStringList){"WXXX"} + text_frames;

                foreach (const QString & name, supported) {
                    if (data.contains(name)) {
                        tag->removeFrames(name.toUtf8().constData());
                    }
                }

                foreach (const QString & name, text_frames) {
                    if (data.contains(name)) {
                        foreach (const QVariant & item, data[name]) {
                            auto frame = new TagLib::ID3v2::TextIdentificationFrame(name.toUtf8().data());
                            frame->setText(item.toString().toStdString());
                            tag->addFrame(frame);
                        }
                    }
                }

                if (data.contains("TRCK")) {
                    auto frame = new TagLib::ID3v2::TextIdentificationFrame("TRCK");
                    frame->setText(data["TRCK"].first().toString().toStdString());
                    tag->removeFrames("TRCK");
                    tag->addFrame(frame);
                }

                if (data.contains("WXXX")) {
                    foreach (const QVariant & url, data["WXXX"]) {
                        auto frame = new TagLib::ID3v2::UserUrlLinkFrame;
                        frame->setText(url.toString().toStdString());
                        tag->addFrame(frame);
                    }
                }
            }
        }
    }
}
