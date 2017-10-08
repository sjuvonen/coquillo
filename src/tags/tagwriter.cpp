#include <taglib/fileref.h>
#include <taglib/flacfile.h>
#include <taglib/mpegfile.h>
#include <taglib/oggfile.h>
#include <taglib/tfile.h>
#include <taglib/vorbisfile.h>
#include <taglib/id3v1tag.h>

#include "crawler/tags.hpp"
#include "tagwriter.hpp"
#include "tagwriterjob.hpp"

#include <QSettings>
#include <QThreadPool>
#include <QDebug>

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
            _progress = 0;

            emit started();
            processItem();
        }

        void Writer::abort() {
            _aborted = true;
            emit aborted();
            emit this->finished();
        }

        void Writer::processItem() {
            _progress++;

            if (_aborted) {
                return;
            }

            if (_items.isEmpty()) {
                emit progress(_progress);
                emit finished();
                return;
            }


            if (_progress % 10 == 0) {
                emit progress(_progress);
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
            QList<QVariantHash> images;
            QSettings settings;

            for (const Image image : item.images()) {
                QVariantHash data({
                    {"type", QVariant(image.type())},
                    {"mime", QVariant(image.mimeType())},
                    {"description", QVariant(image.description())},
                });

                if (settings.value("Filter/ScaleImages").toBool()) {
                    int w = settings.value("Filter/ScaleImagesWidth", 200).toInt();
                    int h = settings.value("Filter/ScaleImagesHeight", 200).toInt();
                    const QImage imgdata(image.scaled(QSize(w, h)));
                    data.insert("data", QVariant::fromValue<QImage>(imgdata));
                } else {
                    data.insert("data", image.source());
                }

                images << data;
            }

            if (isFlacFile(ref.file())) {
                auto file = dynamic_cast<TagLib::FLAC::File*>(ref.file());

                if (item.hasTag("id3v1")) {
                    Crawler::Tag::Generic().write(file->ID3v1Tag(true), item.tag("id3v1").data());
                }

                if (item.hasTag("id3v2")) {
                    Crawler::Tag::Id3v2 id3v2;
                    id3v2.write(file->ID3v2Tag(true), item.tag("id3v2").data());

                    #if TAGLIB_MINOR_VERSION >= 7 || TAGLIB_MAJOR_VERSION > 1
                    if (!item.hasTag("xiph")) {
                        id3v2.writeImages(file->ID3v2Tag(true), images);
                    }
                    #else
                        id3v2.writeImages(file->ID3v2Tag(true), images);
                    #endif
                }

                if (item.hasTag("xiph")) {
                    Crawler::Tag::XiphComment xiph;
                    xiph.write(file->xiphComment(true), item.tag("xiph").data());

                    #if TAGLIB_MINOR_VERSION >= 7 || TAGLIB_MAJOR_VERSION > 1
                    xiph.writeFlacImages(file->xiphComment(true), images);
                    #endif
                }
            } else if (isMpegFile(ref.file())) {
                auto file = dynamic_cast<TagLib::MPEG::File*>(ref.file());

                if (item.hasTag("id3v1")) {
                    Crawler::Tag::Generic().write(file->ID3v1Tag(true), item.tag("id3v1").data());
                }

                if (item.hasTag("id3v2")) {
                    Crawler::Tag::Id3v2 id3v2;
                    id3v2.write(file->ID3v2Tag(true), item.tag("id3v2").data());
                    id3v2.writeImages(file->ID3v2Tag(true), images);
                }
            } else if (isVorbisFile(ref.file())) {
                auto file = dynamic_cast<const TagLib::Ogg::Vorbis::File*>(ref.file());

                if (item.hasTag("xiph")) {
                    Crawler::Tag::XiphComment xiph;
                    xiph.write(file->tag(), item.tag("xiph").data());

                    #if TAGLIB_MINOR_VERSION >= 7 || TAGLIB_MAJOR_VERSION > 1
                    xiph.writeFlacImages(file->tag(), images);
                    #endif
                }
            } else if (!item.primaryTag().isEmpty()) {
                Crawler::Tag::Generic().write(ref.tag(), item.tag(item.primaryTag()).data());
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
