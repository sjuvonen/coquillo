
#include <QCloseEvent>
#include <QDebug>
#include <QItemSelectionModel>
#include <QNetworkAccessManager>
#include <QPointer>
#include <QSettings>
#include <QSignalMapper>
#include <QSortFilterProxyModel>
#include <QTimer>

#include "actionsignalmapper.hpp"
#include "stringstoremodel.hpp"
#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include "filebrowser/filebrowser.hpp"
#include "metadata/metadatamodel.hpp"
#include "processor/renamewidget.hpp"
#include "processor/parserwidget.hpp"
#include "settings/settingsdialog.hpp"
#include "webtags/tagsearchdialog.hpp"

namespace Coquillo {
    MainWindow::MainWindow(QWidget * parent)
    : QMainWindow(parent) {
        _ui = new Ui::MainWindow;
        _ui->setupUi(this);

        _metaData = new MetaData::MetaDataModel(this);
        _nam = new QNetworkAccessManager(this);

        QSortFilterProxyModel * sort_proxy = new QSortFilterProxyModel(this);
        sort_proxy->setSourceModel(_metaData);
        _ui->metaData->setModel(sort_proxy);

        _ui->tagEditor->setModel(_ui->metaData->model());
        _ui->tagEditor->setSelectionModel(_ui->metaData->selectionModel());

        QSettings * storage = new QSettings("history");
        StringStoreModel * bookmarks = new StringStoreModel("bookmarks", 2, this);
        bookmarks->setStorage(storage);

        StringStoreModel * path_history = new StringStoreModel("directories", this);
        path_history->setStorage(storage);
        path_history->setLimit(100);

        _fileBrowser = new FileBrowser(this);
        _fileBrowser->setBookmarkModel(bookmarks);
        _fileBrowser->setHistoryModel(path_history);
        _fileBrowser->setDirectory(QSettings().value("DefaultLocation").toString());
        _ui->tools->addTab(_fileBrowser, tr("Directories"));

        StringStoreModel * rename_history = new StringStoreModel("rename", this);
        rename_history->setStorage(storage);

        StringStoreModel * parser_history = new StringStoreModel("parser", this);
        parser_history->setStorage(storage);

        _fileRenamer = new Processor::RenameWidget(this);
        _fileRenamer->setHistoryModel(rename_history);
        _fileRenamer->setModel(_ui->metaData->model());
        _fileRenamer->setSelectionModel(_ui->metaData->selectionModel());
        _ui->tools->addTab(_fileRenamer, tr("Rename files"));

        _tagParser = new Processor::ParserWidget(this);
        _tagParser->setHistoryModel(parser_history);
        _tagParser->setModel(_ui->metaData->model());
        _tagParser->setSelectionModel(_ui->metaData->selectionModel());
        _ui->tools->addTab(_tagParser, tr("Parse tags"));

        addAction(_ui->action_Quit);

        connect(_fileBrowser, SIGNAL(recursionEnabled(bool)),
            _metaData, SLOT(setRecursive(bool)));

        connect(_fileBrowser, SIGNAL(pathSelected(QString, bool)),
            _metaData, SLOT(addDirectory(QString)));

        connect(_fileBrowser, SIGNAL(pathUnselected(QString, bool)),
            _metaData, SLOT(removeDirectory(QString)));
        connect(_ui->actionReset, SIGNAL(triggered()), _metaData, SLOT(revert()));
        connect(_ui->actionReload, SIGNAL(triggered()), _metaData, SLOT(reload()));
        connect(_ui->actionSave, SIGNAL(triggered()), _metaData, SLOT(writeToDisk()));

        _ui->metaData->header()->setSectionResizeMode(0, QHeaderView::Fixed);
        _ui->metaData->header()->resizeSection(0, 20);
        _ui->metaData->header()->setSortIndicator(1, Qt::AscendingOrder);
        _ui->metaData->header()->setContextMenuPolicy(Qt::CustomContextMenu);

        connect(_ui->metaData->header(), SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(showHeaderContextMenu(QPoint)));

        QTimer::singleShot(1, this, SLOT(restoreSettings()));
    }

    MainWindow::~MainWindow() {
        delete _ui;
    }

    QMenu * MainWindow::createPopupMenu() {
        QMenu * menu = QMainWindow::createPopupMenu();
        menu->addSeparator();
        QAction * action;
        QMenu * sub;

        action = menu->addAction(tr("Menu Bar"));
        action->setCheckable(true);
        action->setChecked(menuBar()->isVisible());
        connect(action, SIGNAL(triggered(bool)), menuBar(), SLOT(setVisible(bool)));

        action = menu->addAction(tr("Status Bar"));
        action->setCheckable(true);
        action->setChecked(statusBar()->isVisible());
        connect(action, SIGNAL(triggered(bool)), statusBar(), SLOT(setVisible(bool)));

        ActionSignalMapper * style_mapper = new ActionSignalMapper(menu);
        connect(style_mapper, SIGNAL(mapped(int)), SLOT(setToolBarIconStyle(int)));

        ActionSignalMapper * size_mapper = new ActionSignalMapper(menu);
        connect(size_mapper, SIGNAL(mapped(int)), SLOT(setToolBarIconSize(int)));

        menu->addSeparator();

        action = menu->addAction(tr("Lock interface"));
        action->setCheckable(true);
        action->setChecked(!findChildren<QToolBar*>().first()->isMovable());
        connect(action, SIGNAL(triggered(bool)), SLOT(lockInterface(bool)));

        menu->addSeparator();

        sub = menu->addMenu(tr("Text Position"));
        style_mapper->setMapping(sub->addAction(tr("Icons Only")), Qt::ToolButtonIconOnly);
        style_mapper->setMapping(sub->addAction(tr("Text Only")), Qt::ToolButtonTextOnly);
        style_mapper->setMapping(sub->addAction(tr("Text Alongside Icons")), Qt::ToolButtonTextBesideIcon);
        style_mapper->setMapping(sub->addAction(tr("Text Under Icons")), Qt::ToolButtonTextUnderIcon);
        connect(sub, SIGNAL(triggered(QAction*)), style_mapper, SLOT(map(QAction*)));

        sub = menu->addMenu(tr("Icon Size"));
        size_mapper->setMapping(sub->addAction(tr("Default")), -1);
        size_mapper->setMapping(sub->addAction(tr("Small") + " (16x16)"), 16);
        size_mapper->setMapping(sub->addAction(tr("Medium") + " (22x22)"), 22);
        size_mapper->setMapping(sub->addAction(tr("Large") + " (32x32)"), 32);
        connect(sub, SIGNAL(triggered(QAction*)), size_mapper, SLOT(map(QAction*)));

        return menu;
    }

    void MainWindow::setToolBarIconSize(int size) {
        setIconSize(QSize(size, size));
    }

    void MainWindow::setToolBarIconStyle(int style) {
        setToolButtonStyle(static_cast<Qt::ToolButtonStyle>(style));
    }

    void MainWindow::closeEvent(QCloseEvent * event) {
        saveSettings();
        event->accept();
    }

    bool MainWindow::isInterfaceLocked() const {
        return !findChildren<QToolBar*>().first()->isMovable();
    }

    void MainWindow::invertSelection() {
        QAbstractItemModel * model = _ui->metaData->model();
        QItemSelectionModel * selection_model = _ui->metaData->selectionModel();
        const QModelIndex tl = model->index(0, 0);
        const QModelIndex br = model->index(model->rowCount() - 1, model->columnCount() - 1);
        selection_model->select(QItemSelection(tl, br), QItemSelectionModel::Toggle);

        // Make sure the active row stays within the selection so that the
        // tag editor widget won't fall out of sync.
        selection_model->setCurrentIndex(selection_model->selection().indexes().value(0), QItemSelectionModel::NoUpdate);
    }

    void MainWindow::lockInterface(bool state) {
        foreach (QToolBar * bar, findChildren<QToolBar*>()) {
            bar->setMovable(!state);
        }
    }

    void MainWindow::openSettingsDialog() {
        Settings::SettingsDialog dialog(this);
        dialog.exec();
    }

    void MainWindow::openTagSearchDialog() {
        WebTags::TagSearchDialog dialog(this);
        dialog.setModel(_metaData);
        dialog.exec();
        _ui->actionSearch->setChecked(false);
    }

    void MainWindow::selectAll() {
        QAbstractItemModel * model = _ui->metaData->model();
        const QModelIndex tl = model->index(0, 0);
        const QModelIndex br = model->index(model->rowCount() - 1, model->columnCount() - 1);
        _ui->metaData->selectionModel()->select(QItemSelection(tl, br), QItemSelectionModel::Select);
    }

    void MainWindow::showHeaderContextMenu(const QPoint & point) const {
        QHeaderView * header = _ui->metaData->header();
        QMenu menu(header);
        QSignalMapper mapper;
        QMap<QString, QAction *> labels;

        QAction * action = menu.addAction(tr("Show modification indicator"));
        action->setCheckable(true);
        action->setChecked(!header->isSectionHidden(0));
        menu.addSeparator();

        mapper.setMapping(action, 0);
        connect(action, SIGNAL(triggered()), &mapper, SLOT(map()));

        for (int i = 1; i < header->count(); i++) {
            const QString label = header->model()->headerData(i, Qt::Horizontal).toString();
            QAction * action = new QAction(label, &menu);
            action->setCheckable(true);
            action->setChecked(!header->isSectionHidden(i));
            mapper.setMapping(action, i);
            labels[label] = action;

            connect(action, SIGNAL(triggered()), &mapper, SLOT(map()));
        }

        foreach (QAction * action, labels.values()) {
            menu.addAction(action);
        }

        void(QSignalMapper::* mapped_signal)(int) = &QSignalMapper::mapped;

        connect(&mapper, mapped_signal, [=](int section) {
            header->setSectionHidden(section, !header->isSectionHidden(section));
        });

        menu.exec(header->mapToGlobal(point));
    }

    void MainWindow::restoreSettings() {
        QSettings settings;
        lockInterface(settings.value("UI/MainWindow/Locked").toBool());
        resize(settings.value("UI/MainWindow/Size").toSize());
        restoreState(settings.value("UI/MainWindow/State").toByteArray());
        menuBar()->setVisible(settings.value("UI/MainWindow/MenuBar", true).toBool());
        statusBar()->setVisible(settings.value("UI/MainWindow/StatusBar", true).toBool());
        _ui->splitter->restoreState(settings.value("UI/MainSplitter/State").toByteArray());
        _ui->metaData->header()->restoreState(settings.value("UI/MainHeader/State").toByteArray());
        _fileBrowser->setRecursive(settings.value("RecursiveScan").toBool());
    }

    void MainWindow::saveSettings() {
        QSettings settings;
        settings.setValue("RecursiveScan", _fileBrowser->isRecursive());
        settings.setValue("UI/MainWindow/Locked", isInterfaceLocked());
        settings.setValue("UI/MainWindow/Size", size());
        settings.setValue("UI/MainWindow/State", saveState());
        settings.setValue("UI/MainWindow/MenuBar", menuBar()->isVisible());
        settings.setValue("UI/MainWindow/StatusBar", statusBar()->isVisible());
        settings.setValue("UI/MainSplitter/State", _ui->splitter->saveState());
        settings.setValue("UI/MainHeader/State", _ui->metaData->header()->saveState());
    }
}
