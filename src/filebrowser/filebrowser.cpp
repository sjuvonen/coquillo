#include <QAbstractItemModel>
#include <QCompleter>
#include <QDebug>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QInputDialog>
#include <QKeyEvent>
#include <QMenu>
#include <QMouseEvent>
#include <QSortFilterProxyModel>

#include "directorymodel.hpp"
#include "filebrowser.hpp"
#include "ui_filebrowser.h"

namespace Coquillo {
    FileBrowser::FileBrowser(QWidget * parent)
    : QWidget(parent) {

        _ui = new Ui::FileBrowser;
        _ui->setupUi(this);

        _directories = new DirectoryModel(this);
        _ui->browser->setModel(_directories);

        QFileSystemModel * filesystem = new QFileSystemModel(this);
        filesystem->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
        _directories->setSourceModel(filesystem);

        QMenu * menu = new QMenu(this);
        menu->addAction("Nothing here");
        _ui->bookmarks->setMenu(menu);

        connect(_ui->recursive, SIGNAL(toggled(bool)), _directories, SLOT(setRecursiveScanEnabled(bool)));
        connect(_ui->recursive, SIGNAL(toggled(bool)), SIGNAL(recursionEnabled(bool)));
        connect(_directories, SIGNAL(pathChecked(QString, bool)), SIGNAL(pathSelected(QString, bool)));
        connect(_directories, SIGNAL(pathUnchecked(QString, bool)), SIGNAL(pathUnselected(QString, bool)));
//         connect(_ui->path, SIGNAL(returnPressed()), SLOT(changeDirectoryFromText()));
        connect(_ui->directory, SIGNAL(activated(QString)), SLOT(setDirectory(QString)));
        connect(_ui->browser, SIGNAL(activated(QModelIndex)), SLOT(changeDirectoryFromIndex(QModelIndex)));
        connect(this, SIGNAL(directoryChanged(QString)), SLOT(updateToggleBookmarkButton()));

        _ui->path->hide();
        _ui->bookmarks->hide();
        _ui->bookmarks->hide();
        _ui->browser->viewport()->installEventFilter(this);
        _ui->directory->completer()->setCaseSensitivity(Qt::CaseSensitive);
    }

    void FileBrowser::keyReleaseEvent(QKeyEvent * event) {
        if (event->key() == Qt::Key_Back) {
            eventFilter(this, event);
        }
    }

    void FileBrowser::mouseReleaseEvent(QMouseEvent * event) {
        if (event->button() == Qt::BackButton) {
            eventFilter(this, event);
        }
    }

    bool FileBrowser::eventFilter(QObject *, QEvent * event) {
        if (event->type() == QEvent::MouseButtonRelease) {
            QMouseEvent * mouse_event = static_cast<QMouseEvent*>(event);
            if (mouse_event->button() == Qt::BackButton) {
                historyGoBack();
                return true;
            }
        }

        if (event->type() == QEvent::KeyRelease) {
            QKeyEvent * key_event = static_cast<QKeyEvent*>(event);
            if (key_event->key() == Qt::Key_Back) {
                historyGoBack();
                return true;
            }
        }

        return false;
    }

    void FileBrowser::setBookmarkModel(QAbstractItemModel * model) {
        _bookmarks = model;
        _ui->bookmarks->setVisible(model != 0);
        _ui->bookmarks->setVisible(model != 0);
        populateBookmarksMenu();
        updateToggleBookmarkButton();

        connect(model, SIGNAL(dataChanged(QModelIndex, QModelIndex)), SLOT(populateBookmarksMenu()));
        connect(model, SIGNAL(rowsRemoved(QModelIndex, int, int)), SLOT(populateBookmarksMenu()));
    }

    QAbstractItemModel * FileBrowser::bookmarkModel() const {
        return _bookmarks.data();
    }

    void FileBrowser::setHistoryModel(QAbstractItemModel * model) {
        _history = model;
        _ui->directory->setModel(model);
    }

    QAbstractItemModel * FileBrowser::historyModel() const {
        return _history.data();
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

    void FileBrowser::setSelectedDirectories(const QStringList & dirs) {
        uncheckAll();
        const QString p = commonParentDirectory(dirs);
        qDebug() << "cd up" << p;

        if (p.indexOf(directory()) != 0) {
          setDirectory(p);
        }

        _directories->selectPaths(dirs);
    }

    void FileBrowser::setDirectory(const QString & path) {
        if (QFileInfo(path).isDir()) {
            addToHistory(path);
            changeDirectory(path);
        }
    }

    void FileBrowser::changeDirectory(const QString & path) {
        if (QFileInfo(path).isDir()) {
            if (!directory().contains(path)) {
                // If NOT moving upwards in the tree, clear selections
                // so that there are no 'ghosts' left behind.
                uncheckAll();
            }

            _ui->path->setText(path);
            _ui->directory->setCurrentText(path);
            _ui->browser->setRootIndex(_directories->index(path));
            _directories->sourceModel()->setRootPath(path);

            emit directoryChanged(path);
        }
    }

    void FileBrowser::setRecursive(bool state) {
        _ui->recursive->setChecked(state);
    }

    void FileBrowser::uncheckAll() {
        _directories->uncheckAll();
    }

    void FileBrowser::addToHistory(const QString & directory) {
        _ui->directory->insertItem(0, directory);
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

    void FileBrowser::changeDirectoryFromAction(QAction * bookmark) {
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

    void FileBrowser::updateToggleBookmarkButton() {
        _ui->bookmarks->setChecked(findBookmark(directory()) != -1);
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
        } else {
            _ui->bookmarks->setChecked(false);
        }
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

    void FileBrowser::unsetCurrentBookmarked() {
        QAbstractItemModel * model = _bookmarks.data();
        const QString path = QFileInfo(_ui->path->text()).absoluteFilePath();
        const QModelIndexList indices = model->match(model->index(0, 1), Qt::DisplayRole, path, 1, Qt::MatchFixedString);

        if (indices.count() > 0) {
            qDebug() << "Removed bookmark" << path << indices[0].row();
            model->removeRow(indices[0].row());
            model->submit();
        } else {
            qDebug() << "Bookmark not found" << path;
        }
    }

    void FileBrowser::historyGoBack() {
        const QString path = _history.data()->index(1, 0).data().toString();
        if (path.length() > 0) {
            changeDirectory(path);
        }
    }

    QString FileBrowser::commonParentDirectory(QStringList paths) {
        if (!paths.isEmpty()) {
            for (int i = 0; i < paths.count(); i++) {
                paths.replace(i, QFileInfo(paths[i]).absoluteFilePath());
            }

            QDir ref(paths.takeFirst());

            while (ref.cdUp()) {
                bool valid = true;

                foreach (const QString path, paths) {
                    if (path.indexOf(ref.path() + '/') != 0) {
                        valid = false;
                        break;
                    }
                }

                if (valid) {
                    return ref.path();
                }
            }
        }
        return QString();
    }
}
