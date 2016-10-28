#include <QSortFilterProxyModel>
#include <QCloseEvent>
#include <QSettings>
#include <QSignalMapper>

#include "filebrowser/filebrowser.hpp"
#include "settings/settingsdialog.hpp"
#include "tags/tagsmodel.hpp"
#include "itemcountlabel.hpp"
#include "mainwindow.hpp"
#include "stringstoremodel.hpp"
#include "togglewidgetbyaction.hpp"
#include "ui_mainwindow.h"

#include <QDebug>

namespace Coquillo {
    MainWindow::MainWindow(Qt::WindowFlags flags)
    : QMainWindow(0, flags) {
        _ui = new Ui::MainWindow;
        _ui->setupUi(this);

        setupMainView();
        setupTagEditor();
        setupFileBrowser();

        restoreSettings();
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

        _model = new Tags::TagsModel(this);

        auto proxy = new QSortFilterProxyModel(this);
        proxy->setSortRole(Qt::EditRole);
        proxy->setSourceModel(_model);

        _ui->itemView->setModel(proxy);
        _ui->itemView->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
        _ui->itemView->horizontalHeader()->setSectionsMovable(true);

        connect(_ui->itemView->horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(showHeaderContextMenu(QPoint)));

        statusBar()->addPermanentWidget(new ItemCountLabel(proxy));

        new ToggleWidgetByAction(menuBar(), _ui->actionMenubar);
        new ToggleWidgetByAction(statusBar(), _ui->actionStatusbar);
    }

    void MainWindow::setupTagEditor() {
        _ui->tagEditor->setModel(_ui->itemView->model());
        _ui->tagEditor->setSelectionModel(_ui->itemView->selectionModel());
    }

    MainWindow::~MainWindow() {
        delete _ui;
    }

    void MainWindow::closeEvent(QCloseEvent * event) {
        saveSettings();
        event->accept();
    }

    void MainWindow::openSettingsDialog() {
        Settings::SettingsDialog dialog(this);
        dialog.exec();
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

        mapper.setMapping(action, 99);
        connect(action, SIGNAL(triggered()), &mapper, SLOT(map()));

        for (int i = 0; i < header->count(); i++) {
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
    }

    void MainWindow::saveSettings() {
        QSettings settings;
        settings.setValue("UI/MainWindow/Size", size());
        settings.setValue("UI/MainWindow/State", saveState());
        settings.setValue("UI/MainWindow/Splitter", _ui->splitter->saveState());
        settings.setValue("UI/MainWindow/Header", _ui->itemView->horizontalHeader()->saveState());
        settings.setValue("UI/MainWindow/MenuBar", menuBar()->isVisible());
        settings.setValue("UI/MainWindow/StatusBar", statusBar()->isVisible());
        settings.sync();
    }
}
