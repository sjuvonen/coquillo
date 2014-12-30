
#include <QDebug>
#include <taglib/xiphcomment.h>
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

                for (auto i = fields.begin(); i != fields.end(); i++) {
                    const QString field = T2QString(i->first);
                    for (auto j = i->second.begin(); j != i->second.end(); j++) {
                        data[field] << T2QString(*j);
                    }
                }

                return data;
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
        }
    }
}
