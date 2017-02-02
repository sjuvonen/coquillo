#ifndef COQUILLO_TAGEDITOR_AUTONUMBERS_H
#define COQUILLO_TAGEDITOR_AUTONUMBERS_H

#include <QHash>
#include <QModelIndexList>
#include <QMultiHash>
#include <QPointer>

class QAbstractItemModel;

namespace Coquillo {
    namespace TagEditor {
        class AutoNumbers {
            public:
                enum Strategy {ItemOrder, FileNumber, Preserve};

                AutoNumbers() { }
                AutoNumbers(QAbstractItemModel * model);
                void setModel(QAbstractItemModel * model);
                QAbstractItemModel * model() const;
                void autoNumberItems(const QModelIndexList & rows);

                static void autoNumberItems(QAbstractItemModel * model, const QModelIndexList & rows);

            private:
                void generateCache();
                void generateNumbering(const QString & directory, const QModelIndexList & items);
                QPointer<QAbstractItemModel> _model;
                QHash<QString, QModelIndexList> groupByPath(const QModelIndexList & items) const;

                QMultiHash<QString, QPersistentModelIndex> _cache;
        };

        namespace NumberStrategy {
            class AbstractStrategy {
                public:
                    virtual ~AbstractStrategy() { }
                    virtual QMap<int, int> suggestions(const QModelIndexList & items) = 0;
            };

            /**
             * Generates track numbers based on item order in source list.
             */
            class ItemOrderStrategy : public AbstractStrategy {
                public:
                    QMap<int, int> suggestions(const QModelIndexList & items);
            };

            /**
             * Parse track numbers from filenames.
             */
            class FileNumberStrategy : public AbstractStrategy {
                public:
                    QMap<int, int> suggestions(const QModelIndexList & items);
            };

            /**
             * Returns track numbers for items that already have it defined.
             */
            class PreserveOriginalNumbers : public AbstractStrategy {
                public:
                    QMap<int, int> suggestions(const QModelIndexList & items);
            };

        }
    }
}

#endif
