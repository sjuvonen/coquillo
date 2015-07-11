
#include <taglib/tstring.h>
#include "metadata/image.hpp"
#include "default.hpp"

#define Q2TString(str) TagLib::String((str).toUtf8().data(), TagLib::String::UTF8)
#define T2QString(str) QString::fromUtf8((str).toCString(true))

namespace Coquillo {
    namespace MetaData {
        namespace Container {
            Default::Default(TagLib::Tag * tag)
            : _tag(tag) {

            }

            MetaData::Tag Default::read() const {
                MetaData::Tag data;
                data["album"] << T2QString(_tag->album());
                data["artist"] << T2QString(_tag->artist());
                data["comment"] << T2QString(_tag->comment());
                data["genre"] << T2QString(_tag->genre());
                data["number"] << _tag->track();
                data["title"] << T2QString(_tag->title());
                data["year"] << _tag->year();
                return data;
            }

            void Default::write(const MetaData::Tag & data) {
                _tag->setAlbum(Q2TString(data.value("album").value(0).toString()));
                _tag->setArtist(Q2TString(data.value("artist").value(0).toString()));
                _tag->setComment(Q2TString(data.value("comment").value(0).toString()));
                _tag->setGenre(Q2TString(data.value("genre").value(0).toString()));
                _tag->setTrack(data.value("number").value(0).toUInt());
                _tag->setTitle(Q2TString(data.value("title").value(0).toString()));
                _tag->setYear(data.value("year").value(0).toUInt());
            }

            QList<Image> Default::readImages() const {
                return QList<Image>();
            }
        }
    }
}
