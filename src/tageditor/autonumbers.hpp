#ifndef COQUILLO_TAGEDITOR_AUTONUMBERS_H
#define COQUILLO_TAGEDITOR_AUTONUMBERS_H

#include <QHash>
#include <QModelIndexList>
#include <QPointer>

class QAbstractItemModel;

namespace Coquillo {
    namespace TagEditor {
        class AutoNumbers {
            public:
                AutoNumbers() { }
                void setModel(QAbstractItemModel * model);
                QAbstractItemModel * model() const;
                void autoNumberItems(const QModelIndexList & rows);

                static void autoNumberItems(QAbstractItemModel * model, const QModelIndexList & rows);

            private:
                void generateNumbering(const QString & directory, const QModelIndexList & items);
                QPointer<QAbstractItemModel> _model;
                QHash<QString, QModelIndexList> groupByPath(const QModelIndexList & items) const;
        };
    }
}

#endif
