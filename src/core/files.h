#ifndef COQUILLO_FILES_H
#define COQUILLO_FILES_H

#include "media.h"
#include <string>

namespace TagLib {
class File;
}

namespace coquillo::files {
Media read(const std::string &path);

bool try_read_flac(const TagLib::File &source, Media &target);
bool try_read_mpeg(const TagLib::File &source, Media &target);
bool try_read_vorbis(const TagLib::File &source, Media &target);
bool try_read_common(const TagLib::File &source, Media &target);
} // namespace coquillo::files

#endif