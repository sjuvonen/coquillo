
#include <QDebug>
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
            QMap<int, int> numbers;
            numbers.unite(NumberStrategy::ItemOrderStrategy().suggestions(items));
            numbers.unite(NumberStrategy::FileNumberStrategy().suggestions(items));
            numbers.unite(NumberStrategy::PreserveOriginalNumbers().suggestions(items));

            foreach (int i, numbers.keys()) {
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

            QMap<int, int> FileNumberStrategy::suggestions(const QModelIndexList & items) {
                QMap<int, int> numbers;
                QRegExp matcher("^(\\d+)[(\\.)(\\s-\\s)]");

                for (int i = 0; i < items.size(); i++) {
                    const QString filename = items[i].data(TagDataRoles::FileNameRole).toString();
                    if (matcher.indexIn(filename.trimmed()) != -1) {
                        numbers.insert(i, matcher.cap(1).toInt());
                    }
                }

                return numbers;
            }

            QMap<int, int> PreserveOriginalNumbers::suggestions(const QModelIndexList & items) {
                QMap<int, int> numbers;

                for (int i = 0; i < items.size(); i++) {
                    const QVariantHash values = items[i].data(TagDataRoles::ValuesMapRole).toHash();

                    if (values.contains("number")) {
                        int number = values["number"].toInt();

                        if (number > 0) {
                            numbers.insert(i, number);
                        }
                    }
                }

                return numbers;
            }
        }
    }
}
