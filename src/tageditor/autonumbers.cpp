
#include <QDebug>
#include <QFileInfo>
#include "metadata/itemdataroles.hpp"
#include "autonumbers.hpp"

namespace Coquillo {
    namespace TagEditor {
        void AutoNumbers::autoNumberItems(QAbstractItemModel * model, const QModelIndexList & rows) {
            AutoNumbers instance;
            instance.setModel(model);
            instance.autoNumberItems(rows);
        }

        void AutoNumbers::setModel(QAbstractItemModel * model) {
            _model = model;
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
            for (int i = 0; i < items.count(); i++) {
                const QModelIndex trnum = items[i].sibling(items[i].row(), 7);
                const QModelIndex trcnt = items[i].sibling(items[i].row(), 8);
                _model->setData(trnum, i+1);
                _model->setData(trcnt, items.count());
            }
        }

        QHash<QString, QModelIndexList> AutoNumbers::groupByPath(const QModelIndexList & items) const {
            QHash<QString, QModelIndexList> paths;
            foreach (const QModelIndex idx, items) {
                const QString path = QFileInfo(idx.data(MetaData::ModelData::FilePathRole).toString()).absolutePath();
                paths[path].append(idx);
            }
            return paths;
        }
    }
}
