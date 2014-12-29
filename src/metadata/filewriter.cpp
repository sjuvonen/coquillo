
#include <taglib/fileref.h>
#include <taglib/flacfile.h>
#include <taglib/mpegfile.h>
#include <taglib/oggfile.h>
#include <taglib/tfile.h>
#include <taglib/vorbisfile.h>

#include "filewriter.h"
#include "tags/dummy.h"
#include "tags/id3v2.h"
#include "tags/xiphcomment.h"

namespace Coquillo {
    namespace MetaData {
        FileWriter::FileWriter(const QList<MetaData> & files, QObject * parent)
        : QObject(parent), _files(files) {

        }

        void FileWriter::run() {
            foreach (const MetaData & data, _files) {
                const TagLib::FileRef ref(data.path().toUtf8().constData());

                if (isFlacFile(ref.file())) {
                    auto file = dynamic_cast<TagLib::FLAC::File*>(ref.file());

                    if (data.hasTag("id3v2")) {
                        Container::Id3v2(file->ID3v2Tag(true)).write(data.tag("id3v2"));
                    }

                    if (data.hasTag("xiph")) {
                        Container::XiphComment(file->xiphComment(true)).write(data.tag("xiph"));
                    }
                    file->save();
                } else if (isMpegFile(ref.file())) {
                    auto file = dynamic_cast<TagLib::MPEG::File*>(ref.file());
                    int tags = 0;

                    if (data.hasTag("id3v2")) {
                        Container::Id3v2(file->ID3v2Tag(true)).write(data.tag("id3v2"));
                        tags |= TagLib::MPEG::File::ID3v2;
                    }

                    file->save(tags);
                } else if (isVorbisFile(ref.file())) {
                    auto file = dynamic_cast<TagLib::Ogg::Vorbis::File*>(ref.file());

                    if (data.hasTag("xiph")) {
                        Container::XiphComment(file->tag()).write(data.tag("xiph"));
                    }

                    file->save();
                }
            }
        }

        bool FileWriter::isFlacFile(const TagLib::File * file) const {
            return dynamic_cast<const TagLib::FLAC::File*>(file) != 0;
        }

        bool FileWriter::isMpegFile(const TagLib::File * file) const {
            return dynamic_cast<const TagLib::MPEG::File*>(file) != 0;
        }

        bool FileWriter::isVorbisFile(const TagLib::File * file) const {
            return dynamic_cast<const TagLib::Ogg::Vorbis::File*>(file) != 0;
        }
    }
}
