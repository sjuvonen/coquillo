
#include <QDebug>
#include <QFileInfo>

#include <taglib/audioproperties.h>
#include <taglib/fileref.h>
#include <taglib/flacfile.h>
#include <taglib/id3v1tag.h>
#include <taglib/mpegfile.h>
#include <taglib/tfile.h>
#include <taglib/vorbisfile.h>

#include "filereader.h"
#include "metadata.h"
#include "tags/default.h"
#include "tags/id3v2.h"
#include "tags/xiphcomment.h"


#include "metadata/imagecache.h"

namespace Coquillo {
    namespace MetaData {
        FileReader::FileReader(const QStringList & files, QObject * parent)
        : QObject(parent), _files(files) {

        }

        void FileReader::run() {
            /*
             * NOTE: Each MetaData object has to contain at least one tag,
             * even if it's empty, so that the app will know what type of tag
             * new values should be written into.
             */
            foreach (const QString & file, _files) {
                const TagLib::FileRef ref(file.toUtf8().constData());
                const TagLib::AudioProperties * audio = ref.audioProperties();

                Properties props;
                props["length"] = audio->length();
                props["bitrate"] = audio->bitrate();
                props["samplerate"] = audio->sampleRate();
                props["channels"] = audio->channels();

                MetaData meta(file);
                meta.setProperties(props);

                if (isFlacFile(ref.file())) {
                    const auto file = dynamic_cast<TagLib::FLAC::File*>(ref.file());
                    if (file->hasXiphComment()) {
                        const auto reader = Container::XiphComment(file->xiphComment());
                        meta.addTag("xiph", reader.read());
                        meta.setImages(reader.readImages());
                    }
                    if (file->hasID3v2Tag()) {
                        const auto reader = Container::Id3v2(file->ID3v2Tag());
                        meta.addTag("id3v2", reader.read());

                        if (!file->hasXiphComment()) {
                            meta.setImages(reader.readImages());
                        }
                    }
                    if (file->hasID3v1Tag()) {
                        meta.addTag("id3v1", Container::Default(file->ID3v1Tag()).read());
                    }
                    if (meta.tags().isEmpty()) {
                        meta.addTag("xiph", Tag());
                    }
                } else if (isMpegFile(ref.file())) {
                    const auto file = dynamic_cast<TagLib::MPEG::File*>(ref.file());
                    if (file->hasID3v2Tag()) {
                        const auto reader = Container::Id3v2(file->ID3v2Tag());
                        meta.addTag("id3v2", reader.read());
                        meta.setImages(reader.readImages());
                    }
                    if (file->hasID3v1Tag()) {
                        meta.addTag("id3v1", Container::Default(file->ID3v1Tag()).read());
                    }
                    if (meta.tags().isEmpty()) {
                        meta.addTag("id3v2", Tag());
                    }
                } else if (isVorbisFile(ref.file())) {
                    const auto file = dynamic_cast<TagLib::Ogg::Vorbis::File*>(ref.file());
                    if (file->tag()) {
                        const auto reader = Container::XiphComment(file->tag());
                        meta.addTag("xiph", reader.read());
                        meta.setImages(reader.readImages());
                    } else {
                        meta.addTag("xiph", Tag());
                    }
                } else {
                    meta.addTag("unknown", Container::Default(ref.tag()).read());
                }

                emit resolved(meta);
            }

            qDebug() << "cache size:" << ImageCache::instance()->count();
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
