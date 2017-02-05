
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include "autonumbers.hpp"
#include "tags/tagdataroles.hpp"

namespace Coquillo {
    namespace TagEditor {
        using Coquillo::Tags::TagDataRoles;

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
                generateNumbering(path, groups[path]);
            }
        }

        void AutoNumbers::generateNumbering(const QString & directory, const QModelIndexList & items) {
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

            FileNumberStrategy::FileNumberStrategy(int mode)
            : _mode(mode) {

            }

            QMap<int, int> FileNumberStrategy::suggestions(const QModelIndexList & items) {
                QMap<int, int> numbers;
                QRegExp matcher("^(\\d+)[(\\.)(\\s-\\s)_]");

                for (int i = 0; i < items.size(); i++) {
                    const QString filename = items[i].data(TagDataRoles::FileNameRole).toString();
                    if (matcher.indexIn(filename.trimmed()) != -1) {
                        const QString num = matcher.cap(1);

                        /*
                         * Sometimes number in filenames are presented so that the first digit
                         * is disc number and last two digits the track number.
                         */
                        if (_mode == TrackNumberMode) {
                            if (num.length() == 3) {
                                numbers.insert(i, num.mid(1).toInt());
                            } else {
                                numbers.insert(i, num.toInt());
                            }
                        } else {
                            if (num.length() == 3) {
                                numbers.insert(i, num.mid(0, 1).toInt());
                            } else {
                                // PASS: There is no disc number in this filename.
                                // numbers.insert(i, 1);
                            }
                        }
                    }
                }

                return numbers;
            }

            PreserveOriginalNumbers::PreserveOriginalNumbers(int mode)
            : _mode(mode) {

            }

            QMap<int, int> PreserveOriginalNumbers::suggestions(const QModelIndexList & items) {
                QMap<int, int> numbers;
                const QString field = _mode == DiscNumberMode ? "disc" : "number";

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
        }
    }
}
