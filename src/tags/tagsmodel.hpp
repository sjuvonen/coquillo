#ifndef COQUILLO_TAGS_TAGSMODEL_H
#define COQUILLO_TAGS_TAGSMODEL_H

#include <QAbstractItemModel>
#include <QPointer>
#include "tagstore.hpp"

namespace Coquillo {
    class ProgressListener;

    namespace Tags {
        namespace Mapper {
            class TagMapper;
        }

        class TagsModel : public QAbstractItemModel {
            Q_OBJECT

            public:
                /**
                 * @deprecated
                 */
                enum Fields {
                    FeedbackField = 0,
                    NumberField = 7,
                    PathField = 15,
                    ImageField = 16
                };

                TagsModel(Store * store, ProgressListener * progress, QObject * parent = 0);

                inline QStringList directories() const { return _directories; }

                int columnCount(const QModelIndex & parent = QModelIndex()) const;
                int rowCount(const QModelIndex & parent = QModelIndex()) const;
                QVariant data(const QModelIndex & idx, int role = Qt::DisplayRole) const;
                QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
                QModelIndex index(int row, int col, const QModelIndex & parent = QModelIndex()) const;
                bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());
                bool setData(const QModelIndex & idx, const QVariant & value, int role = Qt::EditRole);

                inline QModelIndex parent(const QModelIndex &) const {
                    return QModelIndex();
                }

            signals:
                /**
                 * @private
                 *
                 * Internal signal to kill jobs.
                 */
                void abortAllJobs();

                void started();
                void progress(int);
                void finished();

            public slots:
                void abort();
                void addPath(const QString & path);
                void addPaths(const QStringList & paths);
                void discardChanges();
                void reload();
                void removeDirectory(const QString & path);
                void revert();
                void setRecursive(bool state) { _recursive = state; }

            private:
                const Container container(const QModelIndex & idx) const;
                const Container container(int row) const;
                QString containedDirectoryForRow(int row) const;
                bool isRowChanged(const QModelIndex & idx) const;
                void rowChanged(const QModelIndex & idx);

                QStringList _labels;
                QStringList _fields;
                QStringList _directories;
                QPointer<Store> _store;
                QPointer<ProgressListener> _progress;
                bool _recursive;
        };
    }
}

#endif
