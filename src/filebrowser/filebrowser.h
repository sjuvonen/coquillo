#ifndef COQUILLO_FILEBROWSER_H
#define COQUILLO_FILEBROWSER_H

#include <QPointer>
#include <QWidget>

class QAbstractItemModel;

namespace Ui {
    class FileBrowser;
};

namespace Coquillo {
    class DirectoryModel;

    class FileBrowser : public QWidget {
        Q_OBJECT

        public:
            FileBrowser(QWidget * parent = 0);
            void setBookmarkModel(QAbstractItemModel * model);
            QAbstractItemModel * bookmarkModel() const;
            void setHistoryModel(QAbstractItemModel * model);
            QAbstractItemModel * historyModel() const;
            QString directory() const;
            bool isRecursive() const;

        signals:
            void directoryChanged(const QString & dir);
            void recursionEnabled(bool state);
            void pathSelected(const QString & path, bool recursive);
            void pathUnselected(const QString & path, bool recursive);

        public slots:
            void cdUp();
            void setDirectory(const QString & path);
            void setRecursive(bool state);
            void uncheckAll();

        private slots:
            void addToHistory(const QString & dir);
            void changeDirectoryFromIndex(const QModelIndex &);
            void changeDirectoryFromText();
            void populateBookmarksMenu();
            void changeDirectoryFromAction(QAction * bookmark);
            void toggleBookmarked(bool state);
            void updateToggleBookmarkButton();

        private:
            void bookmarkCurrentPath();
            int findBookmark(const QString & path) const;
            void unsetCurrentBookmarked();
            Ui::FileBrowser * _ui;
            DirectoryModel * _directories;
            QPointer<QAbstractItemModel> _bookmarks;
            QPointer<QAbstractItemModel> _history;
    };
};

#endif
