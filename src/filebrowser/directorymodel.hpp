#ifndef COQUILLO_DDIRECTORYMODEL_H
#define COQUILLO_DDIRECTORYMODEL_H

#include <QSortFilterProxyModel>

class QFileSystemModel;

namespace Coquillo {
    class DirectoryModel : public QSortFilterProxyModel {
        Q_OBJECT

        public:
            DirectoryModel(QObject * parent = 0);
            QFileSystemModel * sourceModel() const;
            QModelIndex index(const QString & path, int column = 0) const;

            inline bool recursiveScanEnabled() const { return _recursive; }

            Qt::ItemFlags flags(const QModelIndex & idx) const;
            QVariant data(const QModelIndex & idx, int role) const;
            bool setData(const QModelIndex & idx, const QVariant & value, int role=Qt::EditRole);
            bool hasChildren(const QModelIndex & idx) const;

            int columnCount(const QModelIndex & parent = QModelIndex()) const;

        signals:
            void pathChecked(const QString & path, bool recursive);
            void pathUnchecked(const QString & path, bool recursive);

        public slots:
            void selectPath(const QString & path);
            void selectPaths(const QStringList & paths);
            void setRecursiveScanEnabled(bool state) { _recursive = state; }
            void uncheckAll();

        private:
            bool isAncestorChecked(const QModelIndex & idx) const;
            bool isDescendantChecked(const QModelIndex & idx) const;
            void recursiveDataChanged(const QModelIndex & parent);
            QStringList _checked;
            bool _recursive;
    };
};

#endif
