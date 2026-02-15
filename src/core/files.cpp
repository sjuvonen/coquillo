#include "src/core/files.h"
#include <taglib/flacfile.h>
#include <taglib/id3v1tag.h>
#include <taglib/mpegfile.h>
#include <taglib/tfile.h>
#include <taglib/vorbisfile.h>

namespace coquillo::files {

bool try_read_flac(const TagLib::File &source, Media &target) {
    auto file = dynamic_cast<const TagLib::FLAC::File *>(&source);

    if (!file) {
        return false;
    }

    return true;
}

bool try_read_mpeg(const TagLib::File &source, Media &target) {
    return dynamic_cast<const TagLib::MPEG::File *>(&source) != 0;
}

bool try_read_vorbis(const TagLib::File &source, Media &target) {
    return dynamic_cast<const TagLib::Ogg::Vorbis::File *>(&source) != 0;
}

bool try_read_common(const TagLib::File &source, Media &target) { return false; }

} // namespace coquillo::files
