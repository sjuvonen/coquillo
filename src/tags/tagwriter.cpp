#include <taglib/fileref.h>
#include <taglib/flacfile.h>
#include <taglib/mpegfile.h>
#include <taglib/oggfile.h>
#include <taglib/tfile.h>
#include <taglib/vorbisfile.h>

#include "crawler/tags.hpp"
#include "tagwriter.hpp"
#include "tagwriterjob.hpp"

#include <QThreadPool>

namespace Coquillo {
    namespace Tags {
        Writer::Writer(QObject * parent)
        : QObject(parent) {

        }

        void Writer::write(const Container & item) {
            write({item});
        }

        void Writer::write(const QList<Container> & items) {
            _items = items;
            processItem();
        }

        void Writer::processItem() {
            if (_items.isEmpty()) {
                emit finished();
                return;
            }

            const Container item = _items.takeLast();
            auto * job = new WriterJob(item);

            connect(job, SIGNAL(finished()), SLOT(processItem()));

            QThreadPool::globalInstance()->start(job);
        }

        WriterJob::WriterJob(const Container & item)
        : item(item) {
            setAutoDelete(true);
        }

        void WriterJob::run() {
            TagLib::FileRef ref(item.path().toUtf8().constData());

            if (isFlacFile(ref.file())) {
                auto file = dynamic_cast<TagLib::FLAC::File*>(ref.file());

                if (item.hasTag("id3v2")) {
                    Crawler::Tag::Id3v2().write(file->ID3v2Tag(true), item.tag("id3v2").data());
                }

                if (item.hasTag("xiph")) {
                    Crawler::Tag::XiphComment().write(file->xiphComment(true), item.tag("xiph").data());
                }
            } else if (isMpegFile(ref.file())) {
                auto file = dynamic_cast<TagLib::MPEG::File*>(ref.file());

                if (item.hasTag("id3v2")) {
                    Crawler::Tag::Id3v2().write(file->ID3v2Tag(true), item.tag("id3v2").data());
                }
            } else if (isVorbisFile(ref.file())) {
                auto file = dynamic_cast<const TagLib::Ogg::Vorbis::File*>(ref.file());

                if (item.hasTag("xiph")) {
                    Crawler::Tag::XiphComment().write(file->tag(), item.tag("xiph").data());
                }
            }

            ref.save();

            emit finished();
        }

        bool WriterJob::isFlacFile(const TagLib::File * file) const {
            return dynamic_cast<const TagLib::FLAC::File*>(file) != 0;
        }

        bool WriterJob::isMpegFile(const TagLib::File * file) const {
            return dynamic_cast<const TagLib::MPEG::File*>(file) != 0;
        }

        bool WriterJob::isVorbisFile(const TagLib::File * file) const {
            return dynamic_cast<const TagLib::Ogg::Vorbis::File*>(file) != 0;
        }
    }
}
