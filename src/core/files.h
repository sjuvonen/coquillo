#ifndef COQUILLO_FILES_H
#define COQUILLO_FILES_H

#include "media.h"
#include "type.h"
#include <QList>
#include <QVariantHash>
#include <string>
#include <taglib/xiphcomment.h>

namespace TagLib {
class File;
}

namespace TagLib::ID3v2 {
class Tag;
}

namespace TagLib::ID3v1 {
class Tag;
}

namespace coquillo::files {
Coquillo::Media read(const std::string &path);
Coquillo::Type file_type(const TagLib::FileRef &fileRef);

TagLib::StringList standard_fields();

bool try_read_flac(TagLib::File &source, Coquillo::Media &media);
bool try_read_mpeg(TagLib::File &source, Coquillo::Media &media);
bool try_read_vorbis(TagLib::File &source, Coquillo::Media &media);

void read_id3v2(const TagLib::ID3v2::Tag &tag, Coquillo::Media &media);
void read_id3v1(const TagLib::ID3v1::Tag &tag, Coquillo::Media &media);
void read_xiph(const TagLib::Ogg::XiphComment &tag, Coquillo::Media &media);
void read_common(const TagLib::Tag &tag, Coquillo::Media &media, const QString &name = "Common");
} // namespace coquillo::files

#endif