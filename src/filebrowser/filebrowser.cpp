
#include <QFileInfo>
#include <QFileSystemModel>
#include <QStandardPaths>

#include "directorymodel.h"
#include "filebrowser.h"
#include "ui_filebrowser.h"

namespace Coquillo {
    FileBrowser::FileBrowser(QWidget * parent)
    : QWidget(parent) {

        QString home = QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first();

        _ui = new Ui::FileBrowser;
        _ui->setupUi(this);
        _ui->path->setText(home);

        _directories = new DirectoryModel(this);
        _directories->sourceModel()->setRootPath(home);
        _ui->directories->header()->hide();
        _ui->directories->setModel(_directories);
        _ui->directories->setRootIndex(_directories->index(home));

        connect(_ui->recursive, SIGNAL(toggled(bool)), _directories, SLOT(setRecursiveScanEnabled(bool)));
        connect(_ui->recursive, SIGNAL(toggled(bool)), SIGNAL(recursionEnabled(bool)));

        connect(_directories, SIGNAL(pathChecked(QString, bool)), SIGNAL(pathSelected(QString, bool)));
        connect(_directories, SIGNAL(pathUnchecked(QString, bool)), SIGNAL(pathUnselected(QString, bool)));
        connect(_ui->path, SIGNAL(returnPressed()), SLOT(changeDirectoryFromText()));
        connect(_ui->directories, SIGNAL(activated(QModelIndex)), SLOT(changeDirectoryFromIndex(QModelIndex)));
    }

    QString FileBrowser::directory() const {
        return _ui->path->text();
    }

    bool FileBrowser::recursive() const {
        return _ui->recursive->isChecked();
    }

    void FileBrowser::cdUp() {
        QDir dir = QFileInfo(directory()).absoluteDir();
        if (dir.cdUp()) {
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
}
