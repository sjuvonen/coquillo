
#include <taglib/tag.h>
#include <QVariantMap>
#include "generic.hpp"

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

            void Generic::write(const QVariantMap & values) {
                Q_UNUSED(values)
            }
        }
    }
}
