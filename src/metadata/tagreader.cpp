
#include <taglib/id3v1tag.h>
#include <taglib/id3v2tag.h>
#include <taglib/xiphcomment.h>
#include <QDebug>

#include "tagreader.h"

#define T2QString(str) QString::fromUtf8((str).toCString(true))

namespace Coquillo {
    TagReader::TagReader(TagLib::Tag * tag) : _tag(tag) {

    }

    Tag TagReader::readCommon() const {
        Tag tag;
        tag.insert("title", T2QString(_tag->title()));
        tag.insert("artist", T2QString(_tag->artist()));
        tag.insert("album", T2QString(_tag->album()));
        tag.insert("genre", T2QString(_tag->genre()));
        tag.insert("comment", T2QString(_tag->comment()));
        tag.insert("year", _tag->year());
        tag.insert("number", _tag->track());
        return tag;
    }

    Id3v1Reader::Id3v1Reader(TagLib::ID3v1::Tag * tag)
    : TagReader(tag) {

    }

    Tag Id3v1Reader::read() {
        Tag data = readCommon();
        return data;
    }

    Id3v2Reader::Id3v2Reader(TagLib::ID3v2::Tag * tag)
    : TagReader(tag) {

    }

    Tag Id3v2Reader::read() {
        Tag data = readCommon();
        const TagLib::ID3v2::Tag * tag = dynamic_cast<TagLib::ID3v2::Tag*>(this->tag());
        const TagLib::ID3v2::FrameListMap frames = tag->frameListMap();

        if (tag->frameList("TCOM").size() > 0) {
            data.insert("composer", T2QString(frames["TCOM"].front()->toString()));
        }

        if (tag->frameList("TENC").size() > 0) {
            data.insert("encoder", T2QString(frames["TENC"].front()->toString()));
        }

        if (tag->frameList("TOPE").size() > 0) {
            data.insert("original_artist", T2QString(frames["TOPE"].front()->toString()));
        }

        if (tag->frameList("TPE2").size() > 0) {
            data.insert("album_artist", T2QString(frames["TPE2"].front()->toString()));
        }

        if (tag->frameList("TPOS").size() > 0) {
            data.insert("disc", T2QString(frames["TPOS"].front()->toString()).toInt());
        }

        if (tag->frameList("WXXX").size() > 0) {
            data.insert("url", T2QString(frames["WXXX"].front()->toString()).remove(QRegExp("^\\[\\] ")));
        }

        if (tag->frameList("TRCK").size() > 0) {
            const QString track = T2QString(frames["TRCK"].front()->toString());
            const QStringList parts = track.split('/');

            if (parts.count() >= 2) {
                data.insert("total", parts[1].toInt());
            }
        }
        
        TagLib::ID3v2::FrameListMap::ConstIterator i = frames.begin();

        for (; i != frames.end(); i++) {
            const QString field = i->first.data();
            TagLib::ID3v2::FrameList::ConstIterator j = i->second.begin();

            for (; j != i->second.end();j++) {
                const QString value = T2QString((*j)->toString());
                data.insertMulti(field, value);
            }
        }

        return data;
    }

    XiphReader::XiphReader(TagLib::Ogg::XiphComment * tag)
    : TagReader(tag) {

    }

    Tag XiphReader::read() {
        Tag data = readCommon();
        const TagLib::Ogg::XiphComment * tag = dynamic_cast<TagLib::Ogg::XiphComment*>(this->tag());
        const TagLib::Ogg::FieldListMap fields = tag->fieldListMap();

        if (tag->contains("COMPOSER")) {
            data.insert("composer", T2QString(fields["COMPOSER"].front()));
        }

        if (tag->contains("PERFORMER")) {
            data.insert("original_artist", T2QString(fields["PERFORMER"].front()));
        }

        if (tag->contains("LICENSE")) {
            data.insert("url", T2QString(fields["LICENSE"].front()));
        }

        if (tag->contains("DISCNUMBER")) {
            data.insert("disc", T2QString(fields["DISCNUMBER"].front()).toInt());
        }

        if (tag->contains("TRACKTOTAL")) {
            data.insert("total", T2QString(fields["TRACKTOTAL"].front()).toInt());
        }

        if (tag->contains("ENCODED-BY")) {
            data.insert("encoder", T2QString(fields["ENCODED-BY"].front()));
        }

        if (tag->contains("DESCRIPTION")) {
            data.insert("comment", T2QString(fields["DESCRIPTION"].front()));
        }

        TagLib::Ogg::FieldListMap::ConstIterator i = fields.begin();

        for (; i != fields.end(); i++) {
            const QString field = T2QString(i->first);
            TagLib::StringList::ConstIterator j = i->second.begin();

            for (; j != i->second.end(); j++) {
                const QString value = T2QString(*j);
                data.insertMulti(field, value);
            }
        }



        return data;
    }
}
