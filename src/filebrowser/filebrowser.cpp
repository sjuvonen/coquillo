
#include <QDebug>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QMenu>
#include <QStandardPaths>

#include "directorymodel.h"
#include "filebrowser.h"
#include "ui_filebrowser.h"

namespace Coquillo {
    FileBrowser::FileBrowser(QWidget * parent)
    : QWidget(parent) {

        _ui = new Ui::FileBrowser;
        _ui->setupUi(this);

        _directories = new DirectoryModel(this);
        _ui->directories->header()->hide();
        _ui->directories->setModel(_directories);

        QMenu * menu = new QMenu(this);
        menu->addAction("Nothing here");
        _ui->bookmarks->setMenu(menu);

        connect(_ui->recursive, SIGNAL(toggled(bool)), _directories, SLOT(setRecursiveScanEnabled(bool)));
        connect(_ui->recursive, SIGNAL(toggled(bool)), SIGNAL(recursionEnabled(bool)));
        connect(_directories, SIGNAL(pathChecked(QString, bool)), SIGNAL(pathSelected(QString, bool)));
        connect(_directories, SIGNAL(pathUnchecked(QString, bool)), SIGNAL(pathUnselected(QString, bool)));
        connect(_ui->path, SIGNAL(returnPressed()), SLOT(changeDirectoryFromText()));
        connect(_ui->directory, SIGNAL(activated(QString)), SLOT(setDirectory(QString)));
        connect(_ui->directories, SIGNAL(activated(QModelIndex)), SLOT(changeDirectoryFromIndex(QModelIndex)));

        _ui->path->hide();

        const QString home = QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first();
        setDirectory(home);
    }

    QString FileBrowser::directory() const {
        return _ui->path->text();
    }

    bool FileBrowser::isRecursive() const {
        return _ui->recursive->isChecked();
    }

    void FileBrowser::cdUp() {
        QDir dir = QFileInfo(directory()).absoluteDir();
        if (dir.exists()) {
            setDirectory(dir.absolutePath());
        }
    }

    void FileBrowser::setDirectory(const QString & path) {
        if (QFileInfo(path).isDir()) {
            if (!directory().contains(path)) {
                // If NOT moving upwards in the tree, clear selections
                // so that there are no 'ghosts' left behind.
                uncheckAll();
            }

            _ui->path->setText(path);
            _ui->directory->setCurrentText(path);
            _directories->sourceModel()->setRootPath(path);
            _ui->directories->setRootIndex(_directories->index(path));
        }
    }

    void FileBrowser::setRecursive(bool state) {
        _ui->recursive->setChecked(state);
    }

    void FileBrowser::uncheckAll() {
        _directories->uncheckAll();
    }

    void FileBrowser::changeDirectoryFromIndex(const QModelIndex & idx) {
        setDirectory(idx.data(QFileSystemModel::FilePathRole).toString());
    }

    void FileBrowser::changeDirectoryFromText() {
        setDirectory(_ui->path->text());
    }

    void FileBrowser::populateBookmarksMenu() {

    }

    void FileBrowser::toggleBookmarked(bool state) {
        QString path = _ui->path->text();
    }
}
