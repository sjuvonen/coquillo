
#include <QVariantMap>
#include "abstracttag.h"

#define T2QString(str) QString::fromUtf8((str).toCString(true))

namespace Coquillo {
    namespace MetaData {
        namespace Container {
            AbstractTag::AbstractTag(TagLib::Tag * tag)
            : _tag(tag) {

            }

            QVariantMap AbstractTag::readCommon() const {
                QVariantMap tag;
                tag.insert("title", T2QString(_tag->title()));
                tag.insert("artist", T2QString(_tag->artist()));
                tag.insert("album", T2QString(_tag->album()));
                tag.insert("genre", T2QString(_tag->genre()));
                tag.insert("comment", T2QString(_tag->comment()));
                tag.insert("year", _tag->year());
                tag.insert("number", _tag->track());
                return tag;
            }
        }
    }
}
