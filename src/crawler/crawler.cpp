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
#include "tag/id3v2tag.hpp"
#include "tag/xiphcomment.hpp"

namespace Coquillo {
    namespace Crawler {
        QStringList process_dir(const QString & path) {
            return DirectoryReader().read(path);
        }

        QVariantHash process_file(const QString & path) {
            return FileReader().read(path);
        }

        Crawler::Crawler(QObject * parent)
        : QObject(parent), _recursive(false) {

        }

        void Crawler::searchPath(const QString & path) {
            searchPaths({path});
        }

        void Crawler::searchPaths(const QStringList & paths) {
            auto dirs_watcher = new QFutureWatcher<QStringList>(this);
            auto files_watcher = new QFutureWatcher<QVariantHash>(this);

            files_watcher->setPendingResultsLimit(50);
            dirs_watcher->setFuture(QtConcurrent::mapped(paths, &process_dir));

            connect(dirs_watcher, &QFutureWatcher<QStringList>::finished, [dirs_watcher, files_watcher]() {
                files_watcher->setFuture(QtConcurrent::mapped(dirs_watcher->future().result(), &process_file));
                dirs_watcher->deleteLater();
            });

            connect(files_watcher, &QFutureWatcher<QVariantHash>::progressValueChanged, [this](int value) {
                emit(progress(value));
            });

            connect(files_watcher, &QFutureWatcher<QVariantHash>::resultsReadyAt, [this, files_watcher](int a, int b) {
                QList<QVariantHash> items;
                items.reserve(b - a);

                for (; a < b; a++) {
                    items << files_watcher->resultAt(a);
                }

                emit(results(items));
            });

            connect(files_watcher, &QFutureWatcher<QVariantHash>::finished, [this, files_watcher]() {
                emit(finished());
                files_watcher->deleteLater();
            });
        }

        QStringList DirectoryReader::read(const QString & path) {
            QFileInfo info(path);
            if (info.isFile()) {
                return QStringList() << info.absoluteFilePath();
            }
            QStringList files;
            QDirIterator it(path, nameFilters(), QDir::Files, QDirIterator::Subdirectories);

            while (it.hasNext()) {
                it.next();
                files << it.fileInfo().absoluteFilePath();
            }
            return files;
        }

        QStringList DirectoryReader::read(const QStringList & paths) {
            QStringList files;
            foreach (const QString & path, paths) {
                files << read(path);
            }
            return files;
        }

        QStringList DirectoryReader::nameFilters() const {
            TagLib::StringList exts = TagLib::FileRef::defaultFileExtensions();
            QStringList types = QString::fromUtf8(exts.toString("%%*.").toCString(true)).split("%%");
            types << QString("*.%1").arg(types.takeFirst());
            return types;
        }
        QList<QVariantHash> FileReader::readFiles(const QStringList & paths) const {
            QList<QVariantHash> metadata;
            metadata.reserve(paths.count());
            foreach (const QString & path, paths) {
                metadata << read(path);
            }
            return metadata;
        }

        QVariantHash FileReader::read(const QString & path) const {
            QVariantHash data = {{"path", path}};
            QVariantHash tags;
            const TagLib::FileRef ref(path.toUtf8().constData());

            if (isFlacFile(ref.file())) {
                auto file = dynamic_cast<TagLib::FLAC::File*>(ref.file());

                if (file->hasID3v1Tag()) {
                    tags["id3v1"] = Tag::Generic().read(file->ID3v1Tag());
                    data["primary"] = "id3v1";
                }

                if (file->hasID3v2Tag()) {
                    tags["id3v2"] = Tag::Id3v2Tag().read(file->ID3v2Tag());
                    data["primary"] = "id3v2";
                }

                if (file->hasXiphComment()) {
                    tags["xiph"] = Tag::XiphComment().read(file->xiphComment());
                    data["primary"] = "xiph";
                }
            } else if (isMpegFile(ref.file())) {
                auto file = dynamic_cast<TagLib::MPEG::File*>(ref.file());

                if (file->hasID3v1Tag()) {
                    tags["id3v1"] = Tag::Generic().read(file->ID3v1Tag());
                    data["primary"] = "id3v1";
                }

                if (file->hasID3v2Tag()) {
                    tags["id3v2"] = Tag::Id3v2Tag().read(file->ID3v2Tag());
                    data["primary"] = "id3v2";
                }
            } else if (isVorbisFile(ref.file())) {
                auto file = dynamic_cast<const TagLib::Ogg::Vorbis::File*>(ref.file());

                if (file->tag()) {
                    tags["xiph"] = Tag::XiphComment().read(file->tag());
                    data["primary"] = "xiph";
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
    }
}
