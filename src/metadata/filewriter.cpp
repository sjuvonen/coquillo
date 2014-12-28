
#include "filewriter.h"

namespace Coquillo {
    namespace MetaData {
        FileWriter::FileWriter(const QList<MetaData> & files, QObject * parent)
        : QObject(parent), _files(files) {

        }

        void FileWriter::run() {

        }
    }
}
