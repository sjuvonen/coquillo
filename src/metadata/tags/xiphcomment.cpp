
#include <QDebug>
#include <taglib/xiphcomment.h>
#include "metadata/image.h"
#include "metadata/mapper.h"
#include "xiphcomment.h"

#define Q2TString(str) TagLib::String((str).toUtf8().data(), TagLib::String::UTF8)
#define T2QString(str) QString::fromUtf8((str).toCString(true))

namespace Coquillo {
    namespace MetaData {
        namespace Container {
            XiphComment::XiphComment(TagLib::Ogg::XiphComment * tag)
            : Default(tag) {

            }

            Tag XiphComment::read() const {
                Tag data;
                const auto tag = dynamic_cast<TagLib::Ogg::XiphComment*>(_tag);
                const auto fields = tag->fieldListMap();
                const QStringList ignored = {"METADATA_BLOCK_PICTURE"};

                for (auto i = fields.begin(); i != fields.end(); i++) {
                    const QString field = T2QString(i->first);
                    if (!ignored.contains(field)) {
                        for (auto j = i->second.begin(); j != i->second.end(); j++) {
                            data[field] << T2QString(*j);
                        }
                    }
                }

                return data;
            }

            ImageList XiphComment::readLegacyImages() const {
                const auto tag = dynamic_cast<TagLib::Ogg::XiphComment*>(_tag);
                const auto fields = tag->fieldListMap();
                ImageList images;

                if (fields.contains("COVERART")) {
                    const auto covers = fields["COVERART"];
                    const auto descr = fields["COVERARTDESCRIPTION"];
                    const auto mimes = fields["COVERARTMIME"];
                    for (uint i = 0; i < covers.size(); i++) {
                        const QByteArray data = QByteArray::fromRawData(covers[i].toCString(), covers[i].size());
                        const QImage source = QImage::fromData(QByteArray::fromBase64(data));
                        Image image;
                        image.setSource(source);
                        image.setDescription(T2QString(descr[i]));
                        image.setMimeType(T2QString(mimes[i]));
                        images << image;
                    }
                }

                qDebug() << "xiph legacy images:" << images.count();
                return images;
            }

            ImageList XiphComment::readImages() const {
                const auto tag = dynamic_cast<TagLib::Ogg::XiphComment*>(_tag);
                const auto fields = tag->fieldListMap();
                const char * key = "METADATA_BLOCK_PICTURE";
                ImageList images;

                if (fields.contains(key)) {
                    auto blocks = fields[key];
                    for (auto i = blocks.begin(); i != blocks.end(); i++) {
                        const QByteArray data = QByteArray::fromBase64(i->toCString());
                        const Image image = parseImage(data);

                        if (!image.isNull()) {
                            images << image;
                        }
                    }
                }

                qDebug() << "xiph images:" << images.count();

                return images;
            }

            void XiphComment::write(const Tag & orig) {
                qDebug() << "write xiph" << orig;

                Tag data(orig);
                const XiphMapper mapper;
                const Tag common = {
                    {"album", {mapper.take(data, "album")}},
                    {"artist", {mapper.take(data, "artist")}},
                    {"comment", {mapper.take(data, "comment")}},
                    {"genre", {mapper.take(data, "genre")}},
                    {"number", {mapper.take(data, "number")}},
                    {"title", {mapper.take(data, "title")}},
                    {"year", {mapper.take(data, "year")}},
                };
                Default::write(common);

                auto tag = dynamic_cast<TagLib::Ogg::XiphComment*>(_tag);
                const QStringList fields = {
                    "ALBUM ARTIST",
                    "COMPOSER",
                    "ENCODED-BY",
                    "DISCNUMBER",
                    "LICENSE",
                    "PERFORMER",
                    "TRACKTOTAL",
                };

                foreach (const QString & name, fields) {
                    if (data.contains(name)) {
                        tag->addField(Q2TString(name), Q2TString(data[name].value(0).toString()));
                    }
                }
            }

            Image XiphComment::parseImage(const QByteArray & data) {
                QDataStream s(data);

                int type;
                uint mimelen;
                int descrlen;
                int datalen;

                int w;
                int h;
                int c;
                int ic;

                char * mime;
                char * descr;
                char * pic;

                s >> type;
                s >> mimelen;

                mime = new char[mimelen+1];
                s.readRawData(mime, mimelen);

                mime[mimelen] = 0;

                s >> descrlen;

                descr = new char[descrlen+1];
                s.readRawData(descr, descrlen);

                descr[descrlen] = 0;

                s >> w >> h >> c >> ic >> datalen;


                if (datalen) {
                    pic = new char[datalen];
                    s.readRawData(pic, datalen);
                    Image image;
                    image.setSource(QImage::fromData(QByteArray(pic, datalen)));
                    image.setDescription(QString::fromUtf8(descr, descrlen));
                    image.setMimeType(QString::fromUtf8(mime, mimelen));
                    return image;
                } else {
                    return Image();
                }
            }
        }
    }
}
