#ifndef COQUILLO_METADATA_FILEWRITER_H
#define COQUILLO_METADATA_FILEWRITER_H

#include <QObject>
#include <QRunnable>
#include "metadata.h"

namespace Coquillo {
    namespace MetaData {
        class MetaData;

        class FileWriter : public QObject, public QRunnable {
            Q_OBJECT
            public:
                FileWriter(const QList<MetaData> & files, QObject * parent = 0);
                void run();

            private:
                QList<MetaData> _files;
        };
    }
}

#endif
