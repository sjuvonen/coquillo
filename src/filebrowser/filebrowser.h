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
            void changeDirectoryFromIndex(const QModelIndex &);
            void changeDirectoryFromText();
            void populateBookmarksMenu();
            void setDirectoryFromAction(QAction * bookmark);
            void toggleBookmarked(bool state);
            void updateToggleBookmarkButton();

        private:
            void bookmarkCurrentPath();
            void unsetCurrentBookmarked();
            int findBookmark(const QString & path) const;
            Ui::FileBrowser * _ui;
            DirectoryModel * _directories;
            QPointer<QAbstractItemModel> _bookmarks;
            QPointer<QAbstractItemModel> _history;
    };
};

#endif
