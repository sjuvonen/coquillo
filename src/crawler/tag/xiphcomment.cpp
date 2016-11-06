
#include <taglib/xiphcomment.h>
#include <QImage>
#include "xiphcomment.hpp"

#include <QDebug>

#define Q2TString(str) TagLib::String((str).toUtf8().data(), TagLib::String::UTF8)
#define T2QString(str) QString::fromUtf8((str).toCString(true))

namespace Coquillo {
    namespace Crawler {
        namespace Tag {
            TagData XiphComment::read(const TagLib::Ogg::XiphComment * tag) const {
                const auto fields = tag->fieldListMap();
                const QStringList ignored = {"METADATA_BLOCK_PICTURE"};
                TagData data;

                for (auto i = fields.begin(); i != fields.end(); i++) {
                    const QString field = T2QString(i->first);
                    if (!ignored.contains(field)) {
                        for (auto j = i->second.begin(); j != i->second.end(); j++) {
                            data[field] = T2QString(*j);
                        }
                    }
                }

                return data;
            }

            ImageDataList XiphComment::readImages(const TagLib::Ogg::XiphComment * tag) const {
                const auto fields = tag->fieldListMap();
                const char * key = "METADATA_BLOCK_PICTURE";
                ImageDataList images;

                if (fields.contains(key)) {
                    auto blocks = fields[key];
                    for (auto i = blocks.begin(); i != blocks.end(); i++) {
                        const QByteArray data = QByteArray::fromBase64(i->toCString());
                        const ImageData image = parseImage(data);

                        if (!image.isEmpty()) {
                            images << image;
                        }
                    }
                }

                // qDebug() << "I" << images.size();

                return images;
            }

            #if TAGLIB_MINOR_VERSION >= 7 || TAGLIB_MAJOR_VERSION > 1
            ImageDataList XiphComment::readFlacImages(const TagLib::List<TagLib::FLAC::Picture *> pictures) const {
                ImageDataList images;

                for (auto i = pictures.begin(); i != pictures.end(); i++) {
                    const TagLib::FLAC::Picture * pic = *i;
                    const TagLib::ByteVector imgraw = pic->data();
                    ImageData data;
                    data["data"] = QImage::fromData((const uchar*)imgraw.data(), imgraw.size() );
                    data["description"] = T2QString(pic->description());
                    data["mime"] = T2QString(pic->mimeType());
                    data["type"] = pic->type();

                    images << data;
                }

                return images;
            }
            #endif

            void XiphComment::write(TagLib::Ogg::XiphComment * tag, const TagData & values) {
                const QHash<QString, QString> common_map = {
                    {"album", "ALBUM"},
                    {"artist", "ARTIST"},
                    {"comment", "DESCRIPTION"},
                    {"genre", "GENRE"},
                    {"number", "TRACKNUMBER"},
                    {"title", "TITLE"},
                    {"year", "DATE"},
                };

                QVariantHash common;

                for (auto i = common_map.constBegin(); i != common_map.constEnd(); i++) {
                    if (values.contains(i.value())) {
                        common.insert(i.key(), values[i.value()]);
                    }
                }

                Generic::write(tag, common);

                const QStringList supported = {
                    "ALBUMARTIST",
                    "COMPOSER",
                    "CONTACT",
                    "ENCODED-BY",
                    "DISCNUMBER",
                    "LICENSE",
                    "PERFORMER",
                    "TRACKTOTAL",
                };

                foreach (const QString & key, supported) {
                    if (values.contains(key)) {
                        tag->addField(Q2TString(key), Q2TString(values[key].toString()));
                    }
                }
            }

            ImageData XiphComment::parseImage(const QByteArray & data) {
                qDebug() << "parse";
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
                    ImageData image;
                    image["data"] = QImage::fromData(QByteArray(pic, datalen));
                    image["description"] = QString::fromUtf8(descr, descrlen);
                    image["mime"] = QString::fromUtf8(mime, mimelen);

                    delete mime;
                    delete descr;
                    delete pic;

                    return image;
                } else {
                    delete mime;
                    delete descr;
                    return ImageData();
                }
            }

            ImageDataList XiphComment::readLegacyImages(const TagLib::Ogg::XiphComment * tag) const {
                const auto fields = tag->fieldListMap();
                ImageDataList images;

                if (fields.contains("COVERART")) {
                    const auto covers = fields["COVERART"];
                    const auto descr = fields["COVERARTDESCRIPTION"];
                    const auto mimes = fields["COVERARTMIME"];

                    for (uint i = 0; i < covers.size(); i++) {
                        const QByteArray data = QByteArray::fromRawData(covers[i].toCString(), covers[i].size());
                        const QImage source = QImage::fromData(QByteArray::fromBase64(data));

                        ImageData image;
                        image["data"] = source;
                        image["description"] = T2QString(descr[i]);
                        image["mime"] = T2QString(mimes[i]);
                        images << image;
                    }
                }

                // qDebug() << "L" << images.size();

                return images;
            }
        }
    }
}
