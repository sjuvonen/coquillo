
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

        void FileReader::finish(const MetaData & metaData) {
            emit finished(metaData);
        }
    }
}
