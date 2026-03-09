#include "src/core/files.h"
#include "src/core/media.h"
#include "src/core/type.h"
#include <QRegularExpression>
#include <iostream>
#include <taglib/flacfile.h>
#include <taglib/id3v1tag.h>
#include <taglib/id3v2tag.h>
#include <taglib/mpegfile.h>
#include <taglib/tfile.h>
#include <taglib/tpropertymap.h>
#include <taglib/urllinkframe.h>
#include <taglib/vorbisfile.h>

/**


    TITLE
    ALBUM
    ARTIST
    ALBUMARTIST
    SUBTITLE
    TRACKNUMBER
    DISCNUMBER
    DATE
    ORIGINALDATE
    GENRE
    COMMENT

Sort names:

    TITLESORT
    ALBUMSORT
    ARTISTSORT
    ALBUMARTISTSORT
    COMPOSERSORT

Credits:

    COMPOSER
    LYRICIST
    CONDUCTOR
    REMIXER
    PERFORMER:<XXXX>

Other tags:

    ISRC
    ASIN
    BPM
    COPYRIGHT
    ENCODEDBY
    MOOD
    COMMENT
    MEDIA
    LABEL
    CATALOGNUMBER
    BARCODE
    RELEASECOUNTRY
    RELEASESTATUS
    RELEASETYPE

MusicBrainz identifiers:

    MUSICBRAINZ_TRACKID
    MUSICBRAINZ_ALBUMID
    MUSICBRAINZ_RELEASEGROUPID
    MUSICBRAINZ_RELEASETRACKID
    MUSICBRAINZ_WORKID
    MUSICBRAINZ_ARTISTID
    MUSICBRAINZ_ALBUMARTISTID
    ACOUSTID_ID
    ACOUSTID_FINGERPRINT
    MUSICIP_PUID
*/

typedef QMultiMap<QString, QString> QStringMap;

namespace coquillo::files {

TagLib::StringList standard_fields() {
    return {
        "TITLE", "ALBUM", "ARTIST", "ALBUMARTIST", "TRACKNUMBER", "DISCNUMBER", "GENRE", "COMMENT",
    };
}

Coquillo::Type file_type(const TagLib::FileRef &fileRef) {
    if (dynamic_cast<const TagLib::MPEG::File *>(fileRef.file()) != 0) {
        return Coquillo::MPEG;
    }

    if (dynamic_cast<const TagLib::Ogg::Vorbis::File *>(fileRef.file()) != 0) {
        return Coquillo::Vorbis;
    }

    if (dynamic_cast<const TagLib::FLAC::File *>(fileRef.file()) != 0) {
        return Coquillo::FLAC;
    }

    return Coquillo::Basic;
}

bool try_read_mpeg(TagLib::File &source, Coquillo::Media &media) {
    auto file = dynamic_cast<TagLib::MPEG::File *>(&source);

    if (!file) {
        return false;
    }

    if (file->hasID3v2Tag()) {
        read_id3v2(*file->ID3v2Tag(), media);
    }

    if (file->hasID3v1Tag()) {
        read_id3v1(*file->ID3v1Tag(), media);
    }

    return true;
}

bool try_read_vorbis(TagLib::File &source, Coquillo::Media &media) {
    auto file = dynamic_cast<TagLib::Ogg::Vorbis::File *>(&source);

    if (!file) {
        return false;
    }

    if (file->tag()) {
        read_xiph(*file->tag(), media);
    }

    return true;
}

bool try_read_flac(TagLib::File &source, Coquillo::Media &media) {
    auto file = dynamic_cast<TagLib::FLAC::File *>(&source);

    if (!file) {
        return false;
    }

    if (file->hasXiphComment()) {
        read_xiph(*file->xiphComment(), media);
    }

    if (file->hasID3v2Tag()) {
        read_id3v2(*file->ID3v2Tag(), media);
    }

    if (file->hasID3v1Tag()) {
        read_id3v1(*file->ID3v1Tag(), media);
    }

    media.setImageCount(media.imageCount() + file->pictureList().size());

    return true;
}

void read_id3v2(const TagLib::ID3v2::Tag &tag, Coquillo::Media &media) {
    const auto frames = tag.frameListMap();
    Coquillo::Tag values;

    for (auto it = frames.begin(); it != frames.end(); it++) {
        // Some apps seem to have added unicode trash at the end of tags.
        const QString field = QString(it->first.data()).mid(0, 4);

        if (it->first == "APIC") {
            media.setImageCount(it->second.size());
        } else {
            for (auto j = it->second.begin(); j != it->second.end(); j++) {
                values.insert(field, TStringToQString((*j)->toString()));

                if (field == "WXXX") {
                    const auto wxxx = dynamic_cast<const TagLib::ID3v2::UserUrlLinkFrame *>(*j);

                    // qDebug() << "WXXX:" << TStringToQString(wxxx->description());
                }
            }
        }
    }

    media.addTag("ID3v2", std::move(values));

    /*
     * Convert legacy genre enum into a string.
     */
    // if (values.contains("TCON")) {
    //     bool ok = false;
    //     // int genre_id = data["TCON"].toInt(&ok);

    //     if (ok) {
    //         // data["TCON"] = T2QString(TagLib::ID3v1::genre(genre_id));
    //     }
    // }

    // if (values.contains("TYER")) {
    //     // data["TDRC"] = data.take("TYER");
    // }
}

void read_id3v1(const TagLib::ID3v1::Tag &tag, Coquillo::Media &media) {
    read_common(tag, media, "ID3v1");
}

void read_xiph(const TagLib::Ogg::XiphComment &tag, Coquillo::Media &media) {
    const auto &fields = tag.fieldListMap();
    Coquillo::Tag values;

    for (auto it = fields.begin(); it != fields.end(); it++) {
        const QString field = TStringToQString(it->first);

        std::cout << "\t" << it->first << std::endl;

        for (auto j = it->second.begin(); j != it->second.end(); j++) {
            values.insert(field, TStringToQString(*j));
        }
    }

    media.setImageCount(tag.complexProperties("PICTURE").size());
    media.addTag("VorbisComment", std::move(values));
}

void read_common(const TagLib::Tag &tag, Coquillo::Media &media, const QString &name) {
    Coquillo::Tag values;

    for (const auto it : tag.properties()) {
        const QString field = TStringToQString(it.first);

        for (auto j = it.second.begin(); j != it.second.end(); j++) {
            values.insert(field, TStringToQString(*j));
        }
    }

    media.addTag(name, std::move(values));
}

} // namespace coquillo::files
