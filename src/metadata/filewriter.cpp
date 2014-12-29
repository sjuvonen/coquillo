
#include <taglib/fileref.h>
#include <taglib/flacfile.h>
#include <taglib/mpegfile.h>
#include <taglib/oggfile.h>
#include <taglib/tfile.h>
#include <taglib/vorbisfile.h>

#include "filewriter.h"

namespace Coquillo {
    namespace MetaData {
        FileWriter::FileWriter(const QList<MetaData> & files, QObject * parent)
        : QObject(parent), _files(files) {

        }

        void FileWriter::run() {
            foreach (const MetaData & data, _files) {
                const TagLib::FileRef ref(data.path().toUtf8().constData());

                if (isFlacFile(ref.file())) {
                    const auto file = dynamic_cast<TagLib::FLAC::File*>(ref.file());

                    if (data.hasTag("id3v2")) {

                    }

                    if (data.hasTag("xiph")) {

                    }
                } else if (isMpegFile(ref.file())) {
                    const auto file = dynamic_cast<TagLib::MPEG::File*>(ref.file());

                    if (data.hasTag("id3v2")) {

                    }
                } else if (isVorbisFile(ref.file())) {
                    const auto file = dynamic_cast<TagLib::Ogg::Vorbis::File*>(ref.file());

                    if (data.hasTag("xiph")) {
                        
                    }
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
