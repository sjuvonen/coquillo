#include <functional>
#include <taglib/fileref.h>
#include <taglib/flacfile.h>
#include <taglib/id3v1tag.h>
#include <taglib/mpegfile.h>
#include <taglib/tfile.h>
#include <taglib/vorbisfile.h>

#include <QtConcurrent>
#include <QDirIterator>
#include <QFutureWatcher>

#include "crawler.hpp"
#include "tag/generic.hpp"
#include "tag/id3v2.hpp"
#include "tag/xiphcomment.hpp"

#include <QDebug>
#include <QThread>

namespace Coquillo {
    namespace Crawler {
        QStringList process_dir(const QString & path, bool recursive) {
            return DirectoryReader().read(path, recursive);
        }

        QStringList process_dirs(const QStringList & paths, bool recursive) {
            return DirectoryReader().read(paths, recursive);
        }

        QVariantHash process_file(const QString & path) {
            return FileReader().read(path);
        }

        Crawler::Crawler(QObject * parent)
        : QObject(parent), _recursive(false) {
            connect(&_stash, SIGNAL(results(const QList<QVariantHash> &)), SIGNAL(results(const QList<QVariantHash> &)));

            connect(this, SIGNAL(finished()), &_stash, SLOT(flush()));
        }

        void Crawler::abort() {
            _aborted = true;
            emit aborted();
            emit finished();
        }

        void Crawler::searchPath(const QString & path) {
            searchPaths({path});
        }

        void Crawler::searchPaths(const QStringList & paths) {
            using namespace std::placeholders;

            auto dirs_watcher = new QFutureWatcher<QStringList>(this);
            auto files_watcher = new QFutureWatcher<QVariantHash>(this);

            connect(this, SIGNAL(aborted()), dirs_watcher, SLOT(cancel()));
            connect(this, SIGNAL(aborted()), files_watcher, SLOT(cancel()));

            files_watcher->setPendingResultsLimit(10);

            // std::function<QStringList(const QString &)> proc_d = std::bind(&process_dir, _1, _recursive);
            // dirs_watcher->setFuture(QtConcurrent::mapped(paths, proc_d));

            std::function<QStringList(const QStringList &)> proc_d = std::bind(&process_dirs, _1, _recursive);
            dirs_watcher->setFuture(QtConcurrent::run(proc_d, paths));

            connect(files_watcher, SIGNAL(progressValueChanged(int)), SIGNAL(progress(int)));
            connect(files_watcher, SIGNAL(progressRangeChanged(int, int)), SIGNAL(rangeChanged(int, int)));

            connect(files_watcher, SIGNAL(finished()), SIGNAL(finished()));
            connect(files_watcher, SIGNAL(finished()), SLOT(deleteLater()));

            connect(dirs_watcher, &QFutureWatcher<QStringList>::finished, [this, dirs_watcher, files_watcher] {
                if (this->_aborted) {
                    return;
                }

                files_watcher->setFuture(QtConcurrent::mapped(dirs_watcher->future().result(), &process_file));
                dirs_watcher->deleteLater();
            });

            connect(files_watcher, &QFutureWatcher<QVariantHash>::resultsReadyAt, [this, files_watcher](int a, int b) {
                QList<QVariantHash> items;
                items.reserve(b - a + 1);

                for (; a < b; a++) {
                    // items << files_watcher->resultAt(a);
                    _stash << files_watcher->resultAt(a);
                }

                // qDebug() << "RESULTS" << b - a;

                // _stash->add(items);

                // emit results(items);
            });

            emit started();
        }

        QStringList DirectoryReader::read(const QString & path, bool recursive) {
            QFileInfo info(path);
            if (info.isFile()) {
                return QStringList() << info.absoluteFilePath();
            }
            QStringList files;
            auto flags = recursive ? QDirIterator::Subdirectories : QDirIterator::NoIteratorFlags;
            QDirIterator it(path, nameFilters(), QDir::Files, flags);

            while (it.hasNext()) {
                it.next();
                files << it.fileInfo().absoluteFilePath();
            }
            return files;
        }

        QStringList DirectoryReader::read(const QStringList & paths, bool recursive) {
            QStringList files;
            foreach (const QString & path, paths) {
                files << read(path, recursive);
            }
            return files;
        }

        QStringList DirectoryReader::nameFilters() const {
            TagLib::StringList exts = TagLib::FileRef::defaultFileExtensions();
            QStringList types = QString::fromUtf8(exts.toString("%%*.").toCString(true)).split("%%");
            types << QString("*.%1").arg(types.takeFirst());
            return types;
        }

        QVariantHash FileReader::read(const QString & path) const {
            QVariantHash data = {{"path", path}};
            QVariantHash tags;
            const TagLib::FileRef ref(path.toUtf8().constData());

            if (isFlacFile(ref.file())) {
                auto file = dynamic_cast<TagLib::FLAC::File*>(ref.file());

                ImageDataList images;

                #if TAGLIB_MINOR_VERSION >= 7 || TAGLIB_MAJOR_VERSION > 1
                images << Tag::XiphComment().readFlacImages(file->pictureList());
                #endif

                if (file->hasID3v1Tag()) {
                    tags["id3v1"] = Tag::Generic().read(file->ID3v1Tag());
                    data["primary"] = "id3v1";
                }

                if (file->hasID3v2Tag()) {
                    Tag::Id3v2 reader;
                    tags["id3v2"] = reader.read(file->ID3v2Tag());
                    data["primary"] = "id3v2";

                    images << reader.readImages(file->ID3v2Tag());
                }

                if (file->hasXiphComment()) {
                    Tag::XiphComment reader;
                    tags["xiph"] = reader.read(file->xiphComment());
                    data["primary"] = "xiph";

                    #if TAGLIB_MINOR_VERSION >= 7 || TAGLIB_MAJOR_VERSION > 1
                    images << reader.readFlacImages(file->xiphComment()->pictureList());
                    #endif

                    images << reader.readImages(file->xiphComment())
                        << reader.readLegacyImages(file->xiphComment());
                }

                data["images"] = QVariant::fromValue<ImageDataList>(images);
            } else if (isMpegFile(ref.file())) {
                auto file = dynamic_cast<TagLib::MPEG::File*>(ref.file());

                if (file->hasID3v1Tag()) {
                    tags["id3v1"] = Tag::Generic().read(file->ID3v1Tag());
                    data["primary"] = "id3v1";
                }

                if (file->hasID3v2Tag()) {
                    Tag::Id3v2 reader;
                    ImageDataList images(reader.readImages(file->ID3v2Tag()));
                    data["images"] = QVariant::fromValue<ImageDataList>(images);
                    tags["id3v2"] = reader.read(file->ID3v2Tag());
                    data["primary"] = "id3v2";
                }
            } else if (isVorbisFile(ref.file())) {
                auto file = dynamic_cast<const TagLib::Ogg::Vorbis::File*>(ref.file());

                if (file->tag()) {
                    Tag::XiphComment reader;

                    tags["xiph"] = reader.read(file->tag());
                    data["primary"] = "xiph";

                    ImageDataList images;

                    #if TAGLIB_MINOR_VERSION >= 7 || TAGLIB_MAJOR_VERSION > 1
                    images << reader.readFlacImages(file->tag()->pictureList());
                    #endif

                    images << reader.readImages(file->tag())
                        << reader.readLegacyImages(file->tag());

                    data["images"] = QVariant::fromValue<ImageDataList>(images);
                }
            } else if (ref.tag()) {
                tags["unknown"] = Tag::Generic().read(ref.tag());
                data["primary"] = "unknown";
            }

            data["tags"] = tags;
            return data;
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

        ResultStash::ResultStash(int batch_size)
        : QObject() {
            setBatchSize(batch_size);
        }

        void ResultStash::add(const QList<QVariantHash> & results) {
            _results << results;

            if (_results.size() >= _limit) {
                flush();
            }
        }

        void ResultStash::add(const QVariantHash & result) {
            _results << result;

            if (_results.size() >= _limit) {
                flush();
            }
        }

        void ResultStash::flush() {
            if (_results.size() > 0) {
                emit results(_results);
                _results.clear();
            }
        }

        void ResultStash::setBatchSize(int batch_size) {
            _limit = batch_size;
            _results.reserve(_limit + 5);
        }

        ResultStash & ResultStash::operator<<(const QVariantHash & result) {
            add(result);
            return *this;
        }
    }
}