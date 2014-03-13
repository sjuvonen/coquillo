
#include <taglib/audioproperties.h>
#include <taglib/tfile.h>

#include <QFileInfo>
#include "filetypes.h"
#include "filereader.h"
#include "metadata.h"

namespace Coquillo {
    namespace MetaData {
        FileReader * FileReader::create(const QString & file) {
            QString suffix = QFileInfo(file).suffix().toLower();
            FileReader * reader = 0;

            if (suffix == "mp3") {
                reader = new MpegReader(file);
            } else if (suffix == "ogg" || suffix == "oga") {
                reader = new OggVorbisReader(file);
            } else if (suffix == "flac") {
                reader = new FlacReader(file);
            }

            return reader;
        }

        FileReader::FileReader(const QString & file, QObject * parent)
        : QObject(parent), _file(file) {

        }

        void FileReader::finish(const TagLib::File & file, MetaData & metaData) {
            const TagLib::AudioProperties * audio = file.audioProperties();

            Properties props;
            props["length"] = audio->length();
            props["bitrate"] = audio->bitrate();
            props["samplerate"] = audio->sampleRate();
            props["channels"] = audio->channels();

            metaData.setProperties(props);
            emit finished(metaData);
        }
    }
}
