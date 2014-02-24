#ifndef COQUILLO_FILEBROWSER_H
#define COQUILLO_FILEBROWSER_H

#include <QWidget>

class QStringListModel;

namespace Ui {
    class FileBrowser;
};

namespace Coquillo {
    class DirectoryModel;

    class FileBrowser : public QWidget {
        Q_OBJECT

        public:
            FileBrowser(QWidget * parent = 0);
            QString directory() const;
            bool isRecursive() const;

        signals:
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
            void toggleBookmarked(bool state);

        private:
            Ui::FileBrowser * _ui;
            DirectoryModel * _directories;
            QStringListModel * _bookmarks;
    };
};

#endif
