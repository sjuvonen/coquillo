#ifndef COQUILLO_FILES_H
#define COQUILLO_FILES_H

#include "media.h"
#include "type.h"
#include <QList>
#include <QVariantHash>
#include <string>
#include <taglib/fileref.h>
#include <taglib/tstringlist.h>

namespace TagLib {
class File;
}

namespace coquillo::files {
Coquillo::Media read(const std::string &path);
Coquillo::Type file_type(const TagLib::FileRef &fileRef);

TagLib::StringList standard_fields();

bool try_read_flac(const TagLib::File &source, QList<QVariantHash> &target);
bool try_read_mpeg(const TagLib::File &source, QList<QVariantHash> &target);
bool try_read_vorbis(const TagLib::File &source, QList<QVariantHash> &target);
bool read_common(const TagLib::File &source, QList<QVariantHash> &target);
} // namespace coquillo::files

#endif