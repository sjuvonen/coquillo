#ifndef COQUILLO_FILEBROWSER_H
#define COQUILLO_FILEBROWSER_H

#include <QWidget>

namespace Ui {
    class FileBrowser;
};

namespace Coquillo {
    class DirectoryModel;

    class FileBrowser : public QWidget {
        Q_OBJECT

        public:
            FileBrowser(QWidget * parent = 0);
            QString path() const;
            bool recursive() const;

        signals:
            void recursionEnabled(bool state);
            void pathSelected(const QString & path, bool recursive);
            void pathUnselected(const QString & path, bool recursive);

        public slots:
            void setDirectory(const QString & path);
            void setRecursive(bool state);

        private slots:
            void changeDirectoryFromText();

        private:
            Ui::FileBrowser * _ui;
            DirectoryModel * _directories;
    };
};

#endif
