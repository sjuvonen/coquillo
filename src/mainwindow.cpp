#include <QSortFilterProxyModel>
#include <QCloseEvent>
#include <QProgressBar>
#include <QSettings>
#include <QSignalMapper>

#include "filebrowser/filebrowser.hpp"
#include "settings/settingsdialog.hpp"
#include "tags/tagsmodel.hpp"
#include "itemcountlabel.hpp"
#include "mainwindow.hpp"
#include "progresslistener.hpp"
#include "stringstoremodel.hpp"
#include "togglewidgetbyaction.hpp"
#include "ui_mainwindow.h"

#include <QDebug>

namespace Coquillo {
    MainWindow::MainWindow(Qt::WindowFlags flags)
    : QMainWindow(0, flags) {
        _ui = new Ui::MainWindow;
        _ui->setupUi(this);

        _progress = new ProgressListener(this);
        _model = new Tags::TagsModel(_progress, this);

        setupMainView();
        setupToolBar();
        setupStatusBar();
        setupTagEditor();
        setupFileBrowser();

        restoreSettings();
    }

    MainWindow::~MainWindow() {
        delete _ui;
    }

    QMenu * MainWindow::createPopupMenu() {
        auto * menu = QMainWindow::createPopupMenu();
        menu->addSeparator();

        auto * action = menu->addAction(tr("Menubar"));
        action->setCheckable(true);
        action->setChecked(menuBar()->isVisible());

        connect(action, SIGNAL(triggered(bool)), menuBar(), SLOT(setVisible(bool)));

        action = menu->addAction(tr("Statusbar"));
        action->setCheckable(true);
        action->setChecked(statusBar()->isVisible());

        connect(action, SIGNAL(triggered(bool)), statusBar(), SLOT(setVisible(bool)));

        menu->addSeparator();

        action = menu->addAction(tr("Lock toolbars"));
        action->setCheckable(true);
        action->setChecked(!findChild<QToolBar*>()->isMovable());

        connect(action, SIGNAL(triggered(bool)), this, SLOT(setInterfaceLocked(bool)));

        return menu;
    }

    void MainWindow::setupFileBrowser() {
        QSettings * storage = new QSettings("history");
        StringStoreModel * bookmarks = new StringStoreModel("bookmarks", 2, this);
        bookmarks->setStorage(storage);

        StringStoreModel * path_history = new StringStoreModel("directories", this);
        path_history->setStorage(storage);
        path_history->setLimit(100);

        _files = new FileBrowser;
        _files->setBookmarkModel(bookmarks);
        _files->setHistoryModel(path_history);
        _files->setDirectory(QSettings().value("DefaultLocation").toString());
        _ui->toolBox->addTab(_files, tr("Files"));

        connect(_files, SIGNAL(recursionEnabled(bool)),
            _model, SLOT(setRecursive(bool)));

        connect(_files, SIGNAL(pathSelected(QString, bool)),
            _model, SLOT(addPath(QString)));

        connect(_files, SIGNAL(pathUnselected(QString, bool)),
            _model, SLOT(removeDirectory(QString)));
    }

    void MainWindow::setupMainView() {
        addAction(_ui->actionQuit);
        addAction(_ui->actionMenubar);

        auto proxy = new QSortFilterProxyModel(this);
        proxy->setSortRole(Qt::EditRole);
        proxy->setSourceModel(_model);

        _ui->itemView->setModel(proxy);

        _ui->itemView->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
        _ui->itemView->horizontalHeader()->setSectionsMovable(true);
        _ui->itemView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
        _ui->itemView->horizontalHeader()->resizeSection(0, 20);

        connect(_ui->itemView->horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(showHeaderContextMenu(QPoint)));

        new ToggleWidgetByAction(menuBar(), _ui->actionMenubar);
    }

    void MainWindow::setupStatusBar() {
        auto * bar = new QProgressBar(this);
        bar->setFixedWidth(140);
        bar->hide();

        auto * label = new ItemCountLabel(_ui->itemView->model());

        connect(_progress, SIGNAL(started()), label, SLOT(hide()));
        connect(_progress, SIGNAL(finished()), label, SLOT(show()));

        connect(_progress, SIGNAL(started()), bar, SLOT(show()));
        connect(_progress, SIGNAL(finished()), bar, SLOT(hide()));
        connect(_progress, SIGNAL(aborted()), bar, SLOT(hide()));
        connect(_progress, SIGNAL(progress(int)), bar, SLOT(setValue(int)));
        connect(_progress, SIGNAL(rangeChanged(int, int)), bar, SLOT(setRange(int, int)));

        statusBar()->addPermanentWidget(label);
        statusBar()->addPermanentWidget(bar);

        new ToggleWidgetByAction(statusBar(), _ui->actionStatusbar);
    }

    void MainWindow::setupTagEditor() {
        _ui->tagEditor->setModel(_ui->itemView->model());
        _ui->tagEditor->setSelectionModel(_ui->itemView->selectionModel());
    }

    void MainWindow::setupToolBar() {
        connect(_ui->actionSave, SIGNAL(triggered()), _model, SLOT(writeToDisk()));
        connect(_ui->actionDiscard, SIGNAL(triggered()), _model, SLOT(discardChanges()));
        connect(_ui->actionReload, SIGNAL(triggered()), _model, SLOT(reload()));
    }

    void MainWindow::closeEvent(QCloseEvent * event) {
        saveSettings();
        event->accept();
    }

    void MainWindow::openSettingsDialog() {
        Settings::SettingsDialog dialog(this);
        dialog.exec();
    }

    void MainWindow::selectAll() {
        QAbstractItemModel * model = _ui->itemView->model();
        const QModelIndex start = model->index(0, 0);
        const QModelIndex end = model->index(model->rowCount() - 1, model->columnCount() - 1);
        _ui->itemView->selectionModel()->select(QItemSelection(start, end), QItemSelectionModel::Select);
    }

    void MainWindow::setInterfaceLocked(bool state) {
        foreach (QToolBar * bar, findChildren<QToolBar*>()) {
            bar->setMovable(!state);
        }
    }

    void MainWindow::showHeaderContextMenu(const QPoint & point) const {
        QHeaderView * header = _ui->itemView->horizontalHeader();
        QSignalMapper mapper;
        QMap<QString, QAction *> labels;

        /*
         * Menu is created as a pointer to allow safe binding of the signal handler onto it.
         * That way we make sure that the listener is destroyed after it is not needed anymore.
         */
        QMenu * menu = new QMenu(header);

        QAction * action = menu->addAction(tr("Show modification indicator"));
        action->setCheckable(true);
        action->setChecked(!header->isSectionHidden(0));
        menu->addSeparator();

        mapper.setMapping(action, 0);
        connect(action, SIGNAL(triggered()), &mapper, SLOT(map()));

        for (int i = 1; i < header->count(); i++) {
            const QString label = header->model()->headerData(i, Qt::Horizontal).toString();
            QAction * action = new QAction(label, menu);
            action->setCheckable(true);
            action->setChecked(!header->isSectionHidden(i));
            mapper.setMapping(action, i);
            labels[label] = action;

            connect(action, SIGNAL(triggered()), &mapper, SLOT(map()));
        }

        foreach (QAction * action, labels.values()) {
            menu->addAction(action);
        }

        void(QSignalMapper::* mapped_signal)(int) = &QSignalMapper::mapped;

        menu->connect(&mapper, mapped_signal, [=](int section) {
            header->setSectionHidden(section, !header->isSectionHidden(section));
        });

        menu->exec(header->mapToGlobal(point));
        menu->deleteLater();
    }

    void MainWindow::restoreSettings() {
        QSettings settings;
        resize(settings.value("UI/MainWindow/Size").toSize());
        restoreState(settings.value("UI/MainWindow/State").toByteArray());
        _ui->splitter->restoreState(settings.value("UI/MainWindow/Splitter").toByteArray());
        _ui->itemView->horizontalHeader()->restoreState(settings.value("UI/MainWindow/Header").toByteArray());
        _ui->itemView->horizontalHeader()->setSectionsMovable(true);

        menuBar()->setVisible(settings.value("UI/MainWindow/MenuBar").toBool());
        statusBar()->setVisible(settings.value("UI/MainWindow/StatusBar").toBool());

        setInterfaceLocked(settings.value("UI/MainWindow/LockToolBars").toBool());
    }

    void MainWindow::saveSettings() {
        QSettings settings;
        settings.setValue("UI/MainWindow/Size", size());
        settings.setValue("UI/MainWindow/State", saveState());
        settings.setValue("UI/MainWindow/Splitter", _ui->splitter->saveState());
        settings.setValue("UI/MainWindow/Header", _ui->itemView->horizontalHeader()->saveState());
        settings.setValue("UI/MainWindow/MenuBar", menuBar()->isVisible());
        settings.setValue("UI/MainWindow/StatusBar", statusBar()->isVisible());
        settings.setValue("UI/MainWindow/LockToolBars", !findChild<QToolBar*>()->isMovable());
        settings.sync();
    }
}
