
#include <taglib/tag.h>
#include <taglib/tpropertymap.h>
#include <QVariantMap>
#include "generic.hpp"

#include <QDebug>

#define Q2TString(str) TagLib::String((str).toUtf8().data(), TagLib::String::UTF8)
#define T2QString(str) QString::fromUtf8((str).toCString(true))

namespace Coquillo {
    namespace Crawler {
        namespace Tag {
            QVariantHash Generic::read(const TagLib::Tag * tag) const {
                return {
                    {"album", T2QString(tag->album())},
                    {"artist", T2QString(tag->artist())},
                    {"comment", T2QString(tag->comment())},
                    {"genre", T2QString(tag->genre())},
                    {"title", T2QString(tag->title())},
                    {"number", tag->track()},
                    {"year", tag->year()},
                };
            }

            void Generic::write(TagLib::Tag * tag, const QVariantHash & values) {
                TagLib::PropertyMap properties;

                for (auto i = values.constBegin(); i != values.constEnd(); i++) {
                    properties[Q2TString(i.key())] = {Q2TString(i.value().toString())};
                }

                tag->setProperties(properties);

                if (values.contains("year")) {
                    tag->setYear(values["year"].toInt());
                }

                if (values.contains("number")) {
                    tag->setTrack(values["number"].toInt());
                }
            }
        }
    }
}
