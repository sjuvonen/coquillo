
#include <taglib/audioproperties.h>
#include <taglib/fileref.h>
#include <taglib/flacfile.h>
#include <taglib/mpegfile.h>
#include <taglib/oggfile.h>
#include <taglib/taglib.h>
#include <taglib/tfile.h>
#include <taglib/vorbisfile.h>

#include <QFileInfo>
#include "filereader.h"
#include "metadata.h"
#include "tagreader.h"

namespace Coquillo {
    namespace MetaData {
        FileReader::FileReader(const QStringList & files, QObject * parent)
        : QObject(parent), _files(files) {

        }

        void FileReader::run() {
            foreach (const QString file, _files) {
                const TagLib::FileRef ref(file.toUtf8().constData());
                const TagLib::AudioProperties * audio = ref.audioProperties();

                Properties props;
                props["length"] = audio->length();
                props["bitrate"] = audio->bitrate();
                props["samplerate"] = audio->sampleRate();
                props["channels"] = audio->channels();

                MetaData meta(file);
                meta.setProperties(props);

                if (isMpegFile(ref.file())) {
                    const auto file = dynamic_cast<TagLib::MPEG::File*>(ref.file());
                    if (file->hasID3v2Tag()) {
                        meta.addTag("id3v2", Id3v2Reader(file->ID3v2Tag()).read());
                    }
                    if (file->hasID3v1Tag()) {
                        meta.addTag("id3v1", Id3v1Reader(file->ID3v1Tag()).read());
                    }
                } else if (isVorbisFile(ref.file())) {
                    const auto file = dynamic_cast<TagLib::Ogg::Vorbis::File*>(ref.file());
                    if (file->tag()) {
                        meta.addTag("xiph", XiphReader(file->tag()).read());
                    }
                } else if (isFlacFile(ref.file())) {
                    const auto file = dynamic_cast<TagLib::FLAC::File*>(ref.file());
                    if (file->hasXiphComment()) {
                        meta.addTag("xiph", XiphReader(file->xiphComment()).read());
                    }
                    if (file->hasID3v2Tag()) {
                        meta.addTag("id3v2", Id3v2Reader(file->ID3v2Tag()).read());
                    }
                    if (file->hasID3v1Tag()) {
                        meta.addTag("id3v1", Id3v1Reader(file->ID3v1Tag()).read());
                    }
                }

                emit resolved(meta);
            }
        }


        bool FileReader::isFlacFile(const TagLib::File * file) const {
            return dynamic_cast<const TagLib::FLAC::File*>(file) != 0;
        }

        bool FileReader::isMpegFile(const TagLib::File * file) const {
            return dynamic_cast<const TagLib::MPEG::File*>(file) != 0;
        }

        bool FileReader::isVorbisFile(const TagLib::File * file) const {
            return dynamic_cast<const TagLib::Ogg::Vorbis::File*>(file) != 0;
        }
    }
}
