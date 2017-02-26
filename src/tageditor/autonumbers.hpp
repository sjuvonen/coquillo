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
                void trackNumbers(const QString & directory, const QModelIndexList & items);
                void discNumbers(const QString & directory, const QModelIndexList & items);
                void trackCounts(const QString & directory, const QModelIndexList & items);

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
                    enum Mode { TrackNumberMode = 0, DiscNumberMode };

                    static FileNumberStrategy discNumberMode();

                    static int numberFromPath(const QString & filename, int mode = -1);

                    FileNumberStrategy(int mode = TrackNumberMode);
                    QMap<int, int> suggestions(const QModelIndexList & items);

                private:
                    int _mode;
            };

            /**
             * Returns track numbers for items that already have it defined.
             */
            class PreserveOriginalNumbers : public AbstractStrategy {
                public:
                    enum Mode { TrackNumberMode = 0, DiscNumberMode, TrackCountMode };

                    static PreserveOriginalNumbers discNumberMode();
                    static PreserveOriginalNumbers trackCountMode();

                    PreserveOriginalNumbers(int mode = TrackNumberMode);
                    QMap<int, int> suggestions(const QModelIndexList & items);

                private:
                    int _mode;
            };

            /**
             * Parse disc numbers from parent folder name.
             */
            class DiscNumberPathNameStrategy : public AbstractStrategy {
                public:
                    QMap<int, int> suggestions(const QModelIndexList & items);
            };

            /**
             * Generate a fallback value for items.
             */
            class DiscNumberFallbackStrategy : public AbstractStrategy {
                public:
                    QMap<int, int> suggestions(const QModelIndexList & items);
            };

            /**
             * Deduce album track count from track data.
             */
            class TrackCountFromMetaData : public AbstractStrategy {
                public:
                    QMap<int, int> suggestions(const QModelIndexList & items);

                private:
                    void buildCache(const QModelIndexList & items);
                    QHash<int, int> _cache;
            };

            class TrackCountFromFolderContents : public AbstractStrategy {
                public:
                    static int fileCount(const QString & dirname, const QString & suffix, const QString & prefix = QString());
                    QMap<int, int> suggestions(const QModelIndexList & items);

                private:
                    QHash<QString, int> _cache;
            };
        }
    }
}

#endif
