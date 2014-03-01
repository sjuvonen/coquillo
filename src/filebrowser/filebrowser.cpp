
#include <QAbstractItemModel>
#include <QDebug>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QInputDialog>
#include <QMenu>
#include <QSortFilterProxyModel>
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
        connect(this, SIGNAL(directoryChanged(QString)), SLOT(updateToggleBookmarkButton()));

        _ui->path->hide();
        _ui->bookmarks->hide();
        _ui->toggleBookmark->hide();

        const QString home = QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first();
        setDirectory(home);
    }

    void FileBrowser::setBookmarkModel(QAbstractItemModel * model) {
        _bookmarks = model;
        _ui->bookmarks->setVisible(model != 0);
        _ui->toggleBookmark->setVisible(model != 0);
        populateBookmarksMenu();
        updateToggleBookmarkButton();

        connect(model, SIGNAL(dataChanged(QModelIndex, QModelIndex)), SLOT(populateBookmarksMenu()));
        connect(model, SIGNAL(rowsRemoved(QModelIndex, int, int)), SLOT(populateBookmarksMenu()));
    }

    QAbstractItemModel * FileBrowser::bookmarkModel() const {
        return _bookmarks.data();
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

            emit directoryChanged(path);
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
        QSortFilterProxyModel proxy;
        proxy.setSourceModel(_bookmarks.data());
        proxy.setSortCaseSensitivity(Qt::CaseInsensitive);
        proxy.sort(0);
        _ui->bookmarks->menu()->clear();

        for (int i = 0; i < proxy.rowCount(); i++) {
            const QString label = proxy.index(i, 0).data().toString();
            const QVariant path = proxy.index(i, 1).data();
            _ui->bookmarks->menu()->addAction(label)->setData(path);
        }
    }

    void FileBrowser::setDirectoryFromAction(QAction * bookmark) {
        const QString path = bookmark->data().toString();
        setDirectory(path);

        qDebug() << "browse to" << bookmark->text() << path;
    }

    void FileBrowser::toggleBookmarked(bool state) {
        if (state) {
            bookmarkCurrentPath();
        } else {
            unsetCurrentBookmarked();
        }
    }

    void FileBrowser::bookmarkCurrentPath() {
        const QFileInfo info(_ui->path->text());
        const QString path = info.absoluteFilePath();
        const QString label = QInputDialog::getText(this, tr("Entry name"),
            tr("Give a name for this entry"), QLineEdit::Normal, info.fileName());

        if (!label.isEmpty()) {
            QAbstractItemModel * model = _bookmarks.data();
            int row = model->rowCount();
            model->insertRow(row);
            model->setData(model->index(row, 0), label);
            model->setData(model->index(row, 1), path);
            model->submit();

            qDebug() << "bookmarked" << label << path;
        }
    }

    void FileBrowser::unsetCurrentBookmarked() {
        QAbstractItemModel * model = _bookmarks.data();
        const QString path = QFileInfo(_ui->path->text()).absoluteFilePath();
        const QModelIndexList indices = model->match(model->index(0, 1), Qt::DisplayRole, path, 1, Qt::MatchFixedString);

        qDebug() << "found matches" << indices.count();

        if (indices.count() > 0) {
            qDebug() << "Removed bookmark" << path << indices[0].row();
            model->removeRow(indices[0].row());
            model->submit();
        } else {
            qDebug() << "Bookmark not found" << path;
        }
    }

    void FileBrowser::updateToggleBookmarkButton() {
        _ui->toggleBookmark->setChecked(findBookmark(directory()) != -1);
    }

    int FileBrowser::findBookmark(const QString & path) const {
        if (!_bookmarks.isNull()) {
            QAbstractItemModel * model = _bookmarks.data();
            const QModelIndexList indices = model->match(model->index(0, 1), Qt::DisplayRole, path, 1, Qt::MatchFixedString);
            return indices.value(0).row();
        } else {
            return -1;
        }
    }
}
