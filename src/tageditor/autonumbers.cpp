
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include "autonumbers.hpp"
#include "tags/tagdataroles.hpp"

namespace Coquillo {
    namespace TagEditor {
        using Coquillo::Tags::TagDataRoles;
        using Coquillo::Tags::TagModelField;

        AutoNumbers::AutoNumbers(QAbstractItemModel * model)
        : _model(model) {
            generateCache();
        }

        void AutoNumbers::autoNumberItems(QAbstractItemModel * model, const QModelIndexList & rows) {
            AutoNumbers instance;
            instance.setModel(model);
            instance.autoNumberItems(rows);
        }

        void AutoNumbers::setModel(QAbstractItemModel * model) {
            _model = model;
            generateCache();
        }

        QAbstractItemModel * AutoNumbers::model() const {
            return _model.data();
        }

        void AutoNumbers::autoNumberItems(const QModelIndexList & items) {
            auto groups = groupByPath(items);

            foreach (const QString path, groups.keys()) {
                const QModelIndexList items = groups[path];
                trackNumbers(path, items);
                discNumbers(path, items);
                trackCounts(path, items);
            }
        }

        void AutoNumbers::trackNumbers(const QString & directory, const QModelIndexList & items) {
            Q_UNUSED(directory)

            QMap<int, int> numbers;
            numbers.unite(NumberStrategy::ItemOrderStrategy().suggestions(items));
            numbers.unite(NumberStrategy::FileNumberStrategy().suggestions(items));
            numbers.unite(NumberStrategy::PreserveOriginalNumbers().suggestions(items));

            foreach (int i, numbers.uniqueKeys()) {
                const QModelIndex idx = items[i].sibling(items[i].row(), Tags::NumberField);
                model()->setData(idx, numbers[i]);
            }
        }

        void AutoNumbers::discNumbers(const QString & directory, const QModelIndexList & items) {
            Q_UNUSED(directory)

            QMap<int, int> numbers;
            numbers.unite(NumberStrategy::DiscNumberFallbackStrategy().suggestions(items));
            numbers.unite(NumberStrategy::DiscNumberPathNameStrategy().suggestions(items));
            numbers.unite(NumberStrategy::FileNumberStrategy::discNumberMode().suggestions(items));
            numbers.unite(NumberStrategy::PreserveOriginalNumbers::discNumberMode().suggestions(items));

            foreach (int i, numbers.uniqueKeys()) {
                const QModelIndex idx = items[i].sibling(items[i].row(), Tags::DiscNumberField);
                model()->setData(idx, numbers[i]);
            }
        }

        void AutoNumbers::trackCounts(const QString & directory, const QModelIndexList & items) {
            Q_UNUSED(directory)

            QMap<int, int> numbers;
            // numbers.unite(NumberStrategy::TrackCountFromMetaData().suggestions(items));
            numbers.unite(NumberStrategy::TrackCountFromFolderContents().suggestions(items));
            numbers.unite(NumberStrategy::PreserveOriginalNumbers::trackCountMode().suggestions(items));

            foreach (int i, numbers.uniqueKeys()) {
                const QModelIndex idx = items[i].sibling(items[i].row(), Tags::TrackCountField);
                model()->setData(idx, numbers[i]);
            }
        }

        QHash<QString, QModelIndexList> AutoNumbers::groupByPath(const QModelIndexList & items) const {
            QHash<QString, QModelIndexList> paths;
            foreach (const QModelIndex idx, items) {
                const QString path = QFileInfo(idx.data(Tags::FilePathRole).toString()).absolutePath();
                paths[path].append(idx);
            }
            return paths;
        }

        void AutoNumbers::generateCache() {
            _cache.clear();

            for (int i = 0; i < model()->rowCount(); i++) {
                const QPersistentModelIndex idx = model()->index(i, 0);
                const QString path = QFileInfo(idx.data(Tags::FilePathRole).toString()).absolutePath();
                _cache.insert(path, idx);
            }
        }

        namespace NumberStrategy {
            QMap<int, int> ItemOrderStrategy::suggestions(const QModelIndexList & items) {
                QMap<int, int> numbers;

                for (int i = 0; i < items.size(); i++) {
                    numbers.insert(i, i + 1);
                }

                return numbers;
            }

            FileNumberStrategy FileNumberStrategy::discNumberMode() {
                return FileNumberStrategy(DiscNumberMode);
            }

            FileNumberStrategy::FileNumberStrategy(int mode)
            : _mode(mode) {

            }

            QMap<int, int> FileNumberStrategy::suggestions(const QModelIndexList & items) {
                QMap<int, int> numbers;
                // QRegExp matcher("^(\\d+)[(\\.)(\\s-\\s)_]");

                for (int i = 0; i < items.size(); i++) {
                    const QString filename = items[i].data(TagDataRoles::FileNameRole).toString();
                    int number = numberFromPath(filename.trimmed(), _mode);

                    if (number > 0) {
                        numbers[i] = number;
                    }

                    // if (number > 0) {
                    //     const QString num = QString::number(number);
                    //
                    //     /*
                    //      * Sometimes number in filenames are presented so that the first digit
                    //      * is disc number and last two digits the track number.
                    //      */
                    //     if (_mode == TrackNumberMode) {
                    //         if (num.length() == 3) {
                    //             numbers.insert(i, num.mid(1).toInt());
                    //         } else {
                    //             numbers.insert(i, num.toInt());
                    //         }
                    //     } else {
                    //         if (num.length() == 3) {
                    //             numbers.insert(i, num.mid(0, 1).toInt());
                    //         } else {
                    //             // PASS: There is no disc number in this filename.
                    //             // numbers.insert(i, 1);
                    //         }
                    //     }
                    // }
                }

                return numbers;
            }

            int FileNumberStrategy::numberFromPath(const QString & filename, int mode) {
                static QRegExp matcher("^(\\d+)[(\\.)(\\s-\\s)_]");

                if (matcher.indexIn(filename) != -1) {
                    int number = matcher.cap(1).toInt();

                    switch (mode) {
                        case TrackNumberMode:
                            return number % 100;

                        case DiscNumberMode:
                            return number / 100;

                        default:
                            return number;
                    }
                }

                return 0;
            }

            PreserveOriginalNumbers PreserveOriginalNumbers::discNumberMode() {
                return PreserveOriginalNumbers(DiscNumberMode);
            }

            PreserveOriginalNumbers PreserveOriginalNumbers::trackCountMode() {
                return PreserveOriginalNumbers(TrackCountMode);
            }

            PreserveOriginalNumbers::PreserveOriginalNumbers(int mode)
            : _mode(mode) {

            }

            QMap<int, int> PreserveOriginalNumbers::suggestions(const QModelIndexList & items) {
                const QHash<int, QString> fields = {
                    {TrackNumberMode, "number"},
                    {DiscNumberMode, "disc"},
                    {TrackCountMode, "total"},
                };
                const QString field = fields[_mode];
                QMap<int, int> numbers;

                for (int i = 0; i < items.size(); i++) {
                    const QVariantHash values = items[i].data(TagDataRoles::ValuesMapRole).toHash();

                    if (values.contains(field)) {
                        int number = values[field].toInt();

                        if (number > 0) {
                            numbers.insert(i, number);
                        }
                    }
                }

                return numbers;
            }

            QMap<int, int> DiscNumberPathNameStrategy::suggestions(const QModelIndexList & items) {
                QMap<int, int> numbers;
                QRegExp matcher("(CD|Disc)\\s*(\\d{1,2})", Qt::CaseInsensitive);

                for (int i = 0; i < items.size(); i++) {
                    const QFileInfo info(items[i].data(TagDataRoles::FilePathRole).toString());
                    const QString dirname = info.dir().dirName();

                    if (matcher.indexIn(dirname) != -1) {
                        numbers.insert(i, matcher.cap(2).toInt());
                    }
                }

                return numbers;
            }


            QMap<int, int> DiscNumberFallbackStrategy::suggestions(const QModelIndexList & items) {
                QMap<int, int> numbers;

                for (int i = 0; i < items.size(); i++) {
                    numbers.insert(i, 1);
                }

                return numbers;
            }

            QMap<int, int> TrackCountFromMetaData::suggestions(const QModelIndexList & items) {
                buildCache(items);
                QMap<int, int> numbers;

                for (int i = 0; i < items.size(); i++) {
                    const QModelIndex & idx = items[i];
                    int disc = idx.sibling(idx.row(), TagModelField::DiscNumberField).data().toInt();
                    numbers.insert(i, _cache[disc]);
                }

                return numbers;
            }

            void TrackCountFromMetaData::buildCache(const QModelIndexList & items) {
                _cache.clear();

                foreach (const QModelIndex & idx, items) {
                    int number = idx.sibling(idx.row(), TagModelField::NumberField).data().toInt();
                    int disc = idx.sibling(idx.row(), TagModelField::DiscNumberField).data().toInt();
                    int total = idx.sibling(idx.row(), TagModelField::TrackCountField).data().toInt();
                    int value = qMax(qMax(number, total), 1);

                    if (!_cache.contains(disc) || _cache[disc] < value) {
                        _cache.insert(disc, value);
                    }
                }
            }

            QMap<int, int> TrackCountFromFolderContents::suggestions(const QModelIndexList & items) {
                _cache.clear();
                QMap<int, int> suggestions;

                for (int i = 0; i < items.size(); i++) {
                    const QModelIndex & idx = items[i];
                    const QFileInfo info = QFileInfo(idx.data(Tags::FilePathRole).toString());
                    const QString filename = info.fileName();
                    const QString dirname = info.absolutePath();

                    if (!_cache.contains(dirname)) {
                        int discnum = FileNumberStrategy::numberFromPath(filename, FileNumberStrategy::DiscNumberMode);
                        if (discnum > 0) {
                            const QString filter = QString("%1*.%2").arg(discnum).arg(info.suffix());
                            const QString cid = QString("%1:%2").arg(dirname).arg(discnum);

                            if (!_cache.contains(cid)) {
                                _cache[cid] = fileCount(dirname, info.suffix(), QString::number(discnum));
                            }
                            suggestions[i] = _cache[cid];
                            continue;
                        }

                        _cache[dirname] = fileCount(dirname, info.suffix());
                    }

                    suggestions[i] = _cache[dirname];
                }

                return suggestions;
            }

            int TrackCountFromFolderContents::fileCount(const QString & dirname, const QString & suffix, const QString & prefix) {
                const QString filter = QString("%1*.%2").arg(prefix, suffix);
                const QStringList files = QDir(dirname).entryList({filter}, QDir::Files);
                int count = files.size();
                int last_num = FileNumberStrategy::numberFromPath(files.last(), FileNumberStrategy::TrackNumberMode);

                return qMax(count, last_num);
            }
        }
    }
}
