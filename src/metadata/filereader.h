#ifndef COQUILLO_FILEREADER_H
#define COQUILLO_FILEREADER_H

#include <QObject>

namespace Coquillo {
    class MetaData;

    class FileReader : public QObject {
        Q_OBJECT

        public:
            static FileReader * create(const QString & file);

            FileReader(const QString & file, QObject * parent = 0);
            virtual ~FileReader() { }

            inline QString path() const { return _file; }
            virtual void read() = 0;

        public slots:
            void start() { read(); }

        signals:
            void finished(const MetaData & metaData);

        protected:
            void finish(const MetaData & metaData);

        private:
            enum FileType { MpegFile, OggFile, FlacFile };
            QString _file;
    };
}

#endif
