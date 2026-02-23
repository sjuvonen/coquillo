#include "src/core/files.h"
#include "src/core/type.h"
#include <taglib/flacfile.h>
#include <taglib/id3v1tag.h>
#include <taglib/mpegfile.h>
#include <taglib/tfile.h>
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

bool try_read_flac(const TagLib::File &source, QList<QVariantHash> &target) {
    auto file = dynamic_cast<const TagLib::FLAC::File *>(&source);

    if (!file) {
        return false;
    }

    return true;
}

bool try_read_mpeg(const TagLib::File &source, QList<QVariantHash> &target) {
    return dynamic_cast<const TagLib::MPEG::File *>(&source) != 0;
}

bool try_read_vorbis(const TagLib::File &source, QList<QVariantHash> &target) {
    return dynamic_cast<const TagLib::Ogg::Vorbis::File *>(&source) != 0;
}

bool read_common(const TagLib::File &source, QList<QVariantHash> &target) {
    auto const tag = source.tag();

    target.append({
        {"album_artist", QVariant()},
        {"album", TStringToQString(tag->album())},
        {"artist", TStringToQString(tag->artist())},
        {"comment", TStringToQString(tag->comment())},
        {"composer", QVariant()},
        {"disc", QVariant()},
        {"encoder", QVariant()},
        {"filename", QVariant()},
        {"genre", TStringToQString(tag->genre())},
        {"original_artist", QVariant()},
        {"title", TStringToQString(tag->title())},
        {"total", QVariant()},
        {"track", tag->track()},
        {"url", QVariant()},
        {"year", tag->year()},
    });

    return true;
}

} // namespace coquillo::files
