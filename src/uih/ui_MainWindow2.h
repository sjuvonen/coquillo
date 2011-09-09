/********************************************************************************
** Form generated from reading UI file 'MainWindow2.ui'
**
** Created: Fri Sep 9 21:19:13 2011
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW2_H
#define UI_MAINWINDOW2_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QTableView>
#include <QtGui/QToolBar>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "DirectorySelectorWidget.h"
#include "DirectorySelectorWidget2.h"
#include "EditorWidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow2
{
public:
    QAction *actionQuit;
    QAction *actionConfigure;
    QAction *actionAbout;
    QAction *actionPrevious;
    QAction *actionNext;
    QAction *actionSelect_all;
    QAction *actionInverse;
    QAction *actionSave;
    QAction *actionReset;
    QAction *actionReloadOrAbort;
    QAction *actionProcessor;
    QAction *actionRecursive_scan;
    QAction *actionMenubar;
    QAction *actionCDDB;
    QAction *actionNoBookmarks;
    QAction *actionBookmark;
    QAction *actionLock_interface;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QWidget *widgetToolbar;
    QHBoxLayout *horizontalLayout;
    QToolButton *buttonPrevious;
    QToolButton *buttonNext;
    QFrame *line_2;
    QToolButton *buttonSave;
    QToolButton *buttonReset;
    QToolButton *buttonReloadOrAbort;
    QFrame *line_3;
    QToolButton *buttonSelect_all;
    QToolButton *buttonInverse;
    QFrame *line;
    QToolButton *buttonProcessor;
    QToolButton *buttonCDDB;
    QSpacerItem *horizontalSpacer_3;
    QToolButton *buttonMenu;
    QTableView *tableItems;
    QMenuBar *menubar;
    QMenu *menuCoquillo;
    QMenu *menuSettings;
    QMenu *menuHelp;
    QStatusBar *statusbar;
    QDockWidget *dockEditor;
    QWidget *widget_3;
    QVBoxLayout *verticalLayout_3;
    EditorWidget *widgetEditor;
    QDockWidget *dockLocation2;
    QWidget *widget;
    QVBoxLayout *verticalLayout_2;
    DirectorySelectorWidget2 *widgetLocation;
    QToolBar *mainToolBar;
    QDockWidget *dockLocation;
    DirectorySelectorWidget *widgetLocationOld;

    void setupUi(QMainWindow *MainWindow2)
    {
        if (MainWindow2->objectName().isEmpty())
            MainWindow2->setObjectName(QString::fromUtf8("MainWindow2"));
        MainWindow2->resize(1027, 575);
        MainWindow2->setContextMenuPolicy(Qt::CustomContextMenu);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resources/Coquillo.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow2->setWindowIcon(icon);
        MainWindow2->setDockNestingEnabled(true);
        actionQuit = new QAction(MainWindow2);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionQuit->setMenuRole(QAction::QuitRole);
        actionConfigure = new QAction(MainWindow2);
        actionConfigure->setObjectName(QString::fromUtf8("actionConfigure"));
        actionConfigure->setMenuRole(QAction::PreferencesRole);
        actionAbout = new QAction(MainWindow2);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionAbout->setMenuRole(QAction::AboutRole);
        actionPrevious = new QAction(MainWindow2);
        actionPrevious->setObjectName(QString::fromUtf8("actionPrevious"));
        actionNext = new QAction(MainWindow2);
        actionNext->setObjectName(QString::fromUtf8("actionNext"));
        actionSelect_all = new QAction(MainWindow2);
        actionSelect_all->setObjectName(QString::fromUtf8("actionSelect_all"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/resources/icons/select_all_48x48.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSelect_all->setIcon(icon1);
        actionInverse = new QAction(MainWindow2);
        actionInverse->setObjectName(QString::fromUtf8("actionInverse"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/resources/icons/select_inverse_48x48.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionInverse->setIcon(icon2);
        actionSave = new QAction(MainWindow2);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionSave->setEnabled(false);
        actionReset = new QAction(MainWindow2);
        actionReset->setObjectName(QString::fromUtf8("actionReset"));
        actionReset->setEnabled(false);
        actionReloadOrAbort = new QAction(MainWindow2);
        actionReloadOrAbort->setObjectName(QString::fromUtf8("actionReloadOrAbort"));
        actionProcessor = new QAction(MainWindow2);
        actionProcessor->setObjectName(QString::fromUtf8("actionProcessor"));
        actionProcessor->setCheckable(true);
        actionRecursive_scan = new QAction(MainWindow2);
        actionRecursive_scan->setObjectName(QString::fromUtf8("actionRecursive_scan"));
        actionRecursive_scan->setCheckable(true);
        actionMenubar = new QAction(MainWindow2);
        actionMenubar->setObjectName(QString::fromUtf8("actionMenubar"));
        actionMenubar->setCheckable(true);
        actionMenubar->setChecked(true);
        actionCDDB = new QAction(MainWindow2);
        actionCDDB->setObjectName(QString::fromUtf8("actionCDDB"));
        actionCDDB->setCheckable(true);
        actionNoBookmarks = new QAction(MainWindow2);
        actionNoBookmarks->setObjectName(QString::fromUtf8("actionNoBookmarks"));
        actionNoBookmarks->setEnabled(false);
        actionBookmark = new QAction(MainWindow2);
        actionBookmark->setObjectName(QString::fromUtf8("actionBookmark"));
        actionLock_interface = new QAction(MainWindow2);
        actionLock_interface->setObjectName(QString::fromUtf8("actionLock_interface"));
        actionLock_interface->setCheckable(true);
        centralwidget = new QWidget(MainWindow2);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(10);
        sizePolicy.setVerticalStretch(10);
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(2, 2, 2, 2);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        widgetToolbar = new QWidget(centralwidget);
        widgetToolbar->setObjectName(QString::fromUtf8("widgetToolbar"));
        widgetToolbar->setContextMenuPolicy(Qt::CustomContextMenu);
        horizontalLayout = new QHBoxLayout(widgetToolbar);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        buttonPrevious = new QToolButton(widgetToolbar);
        buttonPrevious->setObjectName(QString::fromUtf8("buttonPrevious"));
        buttonPrevious->setIconSize(QSize(22, 22));
        buttonPrevious->setAutoRaise(true);

        horizontalLayout->addWidget(buttonPrevious);

        buttonNext = new QToolButton(widgetToolbar);
        buttonNext->setObjectName(QString::fromUtf8("buttonNext"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(buttonNext->sizePolicy().hasHeightForWidth());
        buttonNext->setSizePolicy(sizePolicy1);
        buttonNext->setIconSize(QSize(22, 22));
        buttonNext->setAutoRaise(true);

        horizontalLayout->addWidget(buttonNext);

        line_2 = new QFrame(widgetToolbar);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setMinimumSize(QSize(5, 0));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line_2);

        buttonSave = new QToolButton(widgetToolbar);
        buttonSave->setObjectName(QString::fromUtf8("buttonSave"));
        buttonSave->setEnabled(false);
        buttonSave->setIconSize(QSize(22, 22));
        buttonSave->setAutoRaise(true);

        horizontalLayout->addWidget(buttonSave);

        buttonReset = new QToolButton(widgetToolbar);
        buttonReset->setObjectName(QString::fromUtf8("buttonReset"));
        buttonReset->setEnabled(false);
        buttonReset->setIconSize(QSize(22, 22));
        buttonReset->setAutoRaise(true);

        horizontalLayout->addWidget(buttonReset);

        buttonReloadOrAbort = new QToolButton(widgetToolbar);
        buttonReloadOrAbort->setObjectName(QString::fromUtf8("buttonReloadOrAbort"));
        buttonReloadOrAbort->setIconSize(QSize(22, 22));
        buttonReloadOrAbort->setAutoRaise(true);

        horizontalLayout->addWidget(buttonReloadOrAbort);

        line_3 = new QFrame(widgetToolbar);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setMinimumSize(QSize(5, 0));
        line_3->setFrameShape(QFrame::VLine);
        line_3->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line_3);

        buttonSelect_all = new QToolButton(widgetToolbar);
        buttonSelect_all->setObjectName(QString::fromUtf8("buttonSelect_all"));
        buttonSelect_all->setIcon(icon1);
        buttonSelect_all->setIconSize(QSize(22, 22));
        buttonSelect_all->setAutoRaise(true);

        horizontalLayout->addWidget(buttonSelect_all);

        buttonInverse = new QToolButton(widgetToolbar);
        buttonInverse->setObjectName(QString::fromUtf8("buttonInverse"));
        buttonInverse->setIcon(icon2);
        buttonInverse->setIconSize(QSize(22, 22));
        buttonInverse->setAutoRaise(true);

        horizontalLayout->addWidget(buttonInverse);

        line = new QFrame(widgetToolbar);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line);

        buttonProcessor = new QToolButton(widgetToolbar);
        buttonProcessor->setObjectName(QString::fromUtf8("buttonProcessor"));
        buttonProcessor->setIconSize(QSize(24, 24));
        buttonProcessor->setCheckable(true);
        buttonProcessor->setAutoRaise(true);

        horizontalLayout->addWidget(buttonProcessor);

        buttonCDDB = new QToolButton(widgetToolbar);
        buttonCDDB->setObjectName(QString::fromUtf8("buttonCDDB"));
        buttonCDDB->setIconSize(QSize(24, 24));
        buttonCDDB->setCheckable(true);
        buttonCDDB->setAutoRaise(true);

        horizontalLayout->addWidget(buttonCDDB);

        horizontalSpacer_3 = new QSpacerItem(40, 13, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        buttonMenu = new QToolButton(widgetToolbar);
        buttonMenu->setObjectName(QString::fromUtf8("buttonMenu"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(buttonMenu->sizePolicy().hasHeightForWidth());
        buttonMenu->setSizePolicy(sizePolicy2);
        buttonMenu->setIconSize(QSize(22, 22));
        buttonMenu->setPopupMode(QToolButton::InstantPopup);
        buttonMenu->setAutoRaise(true);

        horizontalLayout->addWidget(buttonMenu);


        verticalLayout->addWidget(widgetToolbar);

        tableItems = new QTableView(centralwidget);
        tableItems->setObjectName(QString::fromUtf8("tableItems"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(5);
        sizePolicy3.setHeightForWidth(tableItems->sizePolicy().hasHeightForWidth());
        tableItems->setSizePolicy(sizePolicy3);
        tableItems->setFrameShape(QFrame::StyledPanel);
        tableItems->setFrameShadow(QFrame::Sunken);
        tableItems->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableItems->setProperty("showDropIndicator", QVariant(true));
        tableItems->setDragEnabled(true);
        tableItems->setDragDropOverwriteMode(false);
        tableItems->setDragDropMode(QAbstractItemView::InternalMove);
        tableItems->setDefaultDropAction(Qt::MoveAction);
        tableItems->setSelectionBehavior(QAbstractItemView::SelectItems);
        tableItems->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        tableItems->setShowGrid(false);
        tableItems->setSortingEnabled(true);
        tableItems->horizontalHeader()->setCascadingSectionResizes(true);
        tableItems->horizontalHeader()->setStretchLastSection(true);
        tableItems->verticalHeader()->setVisible(false);
        tableItems->verticalHeader()->setDefaultSectionSize(24);

        verticalLayout->addWidget(tableItems);

        MainWindow2->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow2);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1027, 18));
        menubar->setContextMenuPolicy(Qt::CustomContextMenu);
        menuCoquillo = new QMenu(menubar);
        menuCoquillo->setObjectName(QString::fromUtf8("menuCoquillo"));
        menuSettings = new QMenu(menubar);
        menuSettings->setObjectName(QString::fromUtf8("menuSettings"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow2->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow2);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow2->setStatusBar(statusbar);
        dockEditor = new QDockWidget(MainWindow2);
        dockEditor->setObjectName(QString::fromUtf8("dockEditor"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(1);
        sizePolicy4.setHeightForWidth(dockEditor->sizePolicy().hasHeightForWidth());
        dockEditor->setSizePolicy(sizePolicy4);
        dockEditor->setMinimumSize(QSize(150, 150));
        dockEditor->setContextMenuPolicy(Qt::CustomContextMenu);
        dockEditor->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        widget_3 = new QWidget();
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        sizePolicy4.setHeightForWidth(widget_3->sizePolicy().hasHeightForWidth());
        widget_3->setSizePolicy(sizePolicy4);
        verticalLayout_3 = new QVBoxLayout(widget_3);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(2, 2, 2, 0);
        widgetEditor = new EditorWidget(widget_3);
        widgetEditor->setObjectName(QString::fromUtf8("widgetEditor"));

        verticalLayout_3->addWidget(widgetEditor);

        dockEditor->setWidget(widget_3);
        MainWindow2->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockEditor);
        dockLocation2 = new QDockWidget(MainWindow2);
        dockLocation2->setObjectName(QString::fromUtf8("dockLocation2"));
        sizePolicy4.setHeightForWidth(dockLocation2->sizePolicy().hasHeightForWidth());
        dockLocation2->setSizePolicy(sizePolicy4);
        dockLocation2->setMinimumSize(QSize(200, 179));
        dockLocation2->setBaseSize(QSize(200, 200));
        dockLocation2->setContextMenuPolicy(Qt::CustomContextMenu);
        dockLocation2->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        widget = new QWidget();
        widget->setObjectName(QString::fromUtf8("widget"));
        sizePolicy4.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy4);
        verticalLayout_2 = new QVBoxLayout(widget);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(2, 2, 2, 2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        widgetLocation = new DirectorySelectorWidget2(widget);
        widgetLocation->setObjectName(QString::fromUtf8("widgetLocation"));
        QSizePolicy sizePolicy5(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(5);
        sizePolicy5.setHeightForWidth(widgetLocation->sizePolicy().hasHeightForWidth());
        widgetLocation->setSizePolicy(sizePolicy5);

        verticalLayout_2->addWidget(widgetLocation);

        dockLocation2->setWidget(widget);
        MainWindow2->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockLocation2);
        mainToolBar = new QToolBar(MainWindow2);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        mainToolBar->setContextMenuPolicy(Qt::CustomContextMenu);
        MainWindow2->addToolBar(Qt::TopToolBarArea, mainToolBar);
        dockLocation = new QDockWidget(MainWindow2);
        dockLocation->setObjectName(QString::fromUtf8("dockLocation"));
        widgetLocationOld = new DirectorySelectorWidget();
        widgetLocationOld->setObjectName(QString::fromUtf8("widgetLocationOld"));
        dockLocation->setWidget(widgetLocationOld);
        MainWindow2->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockLocation);

        menubar->addAction(menuCoquillo->menuAction());
        menubar->addAction(menuSettings->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuCoquillo->addAction(actionQuit);
        menuSettings->addAction(actionMenubar);
        menuSettings->addAction(actionLock_interface);
        menuSettings->addSeparator();
        menuSettings->addAction(actionRecursive_scan);
        menuSettings->addSeparator();
        menuSettings->addAction(actionConfigure);
        menuHelp->addAction(actionAbout);
        mainToolBar->addAction(actionPrevious);
        mainToolBar->addAction(actionNext);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionSave);
        mainToolBar->addAction(actionReset);
        mainToolBar->addAction(actionReloadOrAbort);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionSelect_all);
        mainToolBar->addAction(actionInverse);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionProcessor);
        mainToolBar->addAction(actionCDDB);

        retranslateUi(MainWindow2);
        QObject::connect(actionQuit, SIGNAL(triggered()), MainWindow2, SLOT(close()));
        QObject::connect(actionMenubar, SIGNAL(toggled(bool)), menubar, SLOT(setVisible(bool)));
        QObject::connect(actionSelect_all, SIGNAL(triggered()), tableItems, SLOT(selectAll()));
        QObject::connect(actionLock_interface, SIGNAL(toggled(bool)), MainWindow2, SLOT(setInterfaceLocked(bool)));
        QObject::connect(menubar, SIGNAL(customContextMenuRequested(QPoint)), MainWindow2, SLOT(showToolBarContextMenu(QPoint)));
        QObject::connect(dockEditor, SIGNAL(customContextMenuRequested(QPoint)), MainWindow2, SLOT(showToolBarContextMenu(QPoint)));
        QObject::connect(dockLocation2, SIGNAL(customContextMenuRequested(QPoint)), MainWindow2, SLOT(showToolBarContextMenu(QPoint)));
        QObject::connect(buttonSelect_all, SIGNAL(clicked()), tableItems, SLOT(selectAll()));
        QObject::connect(widgetToolbar, SIGNAL(customContextMenuRequested(QPoint)), MainWindow2, SLOT(showToolBarContextMenu(QPoint)));
        QObject::connect(mainToolBar, SIGNAL(customContextMenuRequested(QPoint)), MainWindow2, SLOT(showToolBarContextMenu(QPoint)));

        QMetaObject::connectSlotsByName(MainWindow2);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow2)
    {
        MainWindow2->setWindowTitle(QApplication::translate("MainWindow2", "Coquillo", 0, QApplication::UnicodeUTF8));
        actionQuit->setText(QApplication::translate("MainWindow2", "Quit", 0, QApplication::UnicodeUTF8));
        actionQuit->setShortcut(QApplication::translate("MainWindow2", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        actionConfigure->setText(QApplication::translate("MainWindow2", "Configure...", 0, QApplication::UnicodeUTF8));
        actionConfigure->setShortcut(QApplication::translate("MainWindow2", "Ctrl+P", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("MainWindow2", "About Coquillo...", 0, QApplication::UnicodeUTF8));
        actionPrevious->setText(QApplication::translate("MainWindow2", "Previous", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionPrevious->setToolTip(QApplication::translate("MainWindow2", "Select previous item", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionPrevious->setShortcut(QApplication::translate("MainWindow2", "PgUp", 0, QApplication::UnicodeUTF8));
        actionNext->setText(QApplication::translate("MainWindow2", "Next", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionNext->setToolTip(QApplication::translate("MainWindow2", "Select next item", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionNext->setShortcut(QApplication::translate("MainWindow2", "PgDown", 0, QApplication::UnicodeUTF8));
        actionSelect_all->setText(QApplication::translate("MainWindow2", "Select all", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionSelect_all->setToolTip(QApplication::translate("MainWindow2", "Select all items", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionSelect_all->setShortcut(QApplication::translate("MainWindow2", "Ctrl+A", 0, QApplication::UnicodeUTF8));
        actionInverse->setText(QApplication::translate("MainWindow2", "Inverse", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionInverse->setToolTip(QApplication::translate("MainWindow2", "Invert selection", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionInverse->setShortcut(QApplication::translate("MainWindow2", "Ctrl+Shift+A", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("MainWindow2", "Save", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionSave->setToolTip(QApplication::translate("MainWindow2", "Save all changes", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionSave->setShortcut(QApplication::translate("MainWindow2", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionReset->setText(QApplication::translate("MainWindow2", "Reset", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionReset->setToolTip(QApplication::translate("MainWindow2", "Revert all changes", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionReloadOrAbort->setText(QApplication::translate("MainWindow2", "Reload", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionReloadOrAbort->setToolTip(QApplication::translate("MainWindow2", "Reload directory", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionReloadOrAbort->setShortcut(QApplication::translate("MainWindow2", "Ctrl+R", 0, QApplication::UnicodeUTF8));
        actionProcessor->setText(QApplication::translate("MainWindow2", "Processor", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionProcessor->setToolTip(QApplication::translate("MainWindow2", "Open the field processor", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionProcessor->setShortcut(QApplication::translate("MainWindow2", "Ctrl+B", 0, QApplication::UnicodeUTF8));
        actionRecursive_scan->setText(QApplication::translate("MainWindow2", "Recursive scan", 0, QApplication::UnicodeUTF8));
        actionMenubar->setText(QApplication::translate("MainWindow2", "Menubar", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionMenubar->setToolTip(QApplication::translate("MainWindow2", "Toggle menubar", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionMenubar->setShortcut(QApplication::translate("MainWindow2", "Ctrl+M", 0, QApplication::UnicodeUTF8));
        actionCDDB->setText(QApplication::translate("MainWindow2", "CDDB", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionCDDB->setToolTip(QApplication::translate("MainWindow2", "Make CDDB searches", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionNoBookmarks->setText(QApplication::translate("MainWindow2", "No bookmarks", 0, QApplication::UnicodeUTF8));
        actionBookmark->setText(QApplication::translate("MainWindow2", "Bookmark", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionBookmark->setToolTip(QApplication::translate("MainWindow2", "Add or remove this directory in bookmarks", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionLock_interface->setText(QApplication::translate("MainWindow2", "Lock interface", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionLock_interface->setToolTip(QApplication::translate("MainWindow2", "Lock toolbars and docks", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionLock_interface->setShortcut(QApplication::translate("MainWindow2", "Ctrl+L", 0, QApplication::UnicodeUTF8));
        buttonPrevious->setText(QApplication::translate("MainWindow2", "Previous", 0, QApplication::UnicodeUTF8));
        buttonNext->setText(QApplication::translate("MainWindow2", "Next", 0, QApplication::UnicodeUTF8));
        buttonSave->setText(QApplication::translate("MainWindow2", "Save", 0, QApplication::UnicodeUTF8));
        buttonReset->setText(QApplication::translate("MainWindow2", "Reset", 0, QApplication::UnicodeUTF8));
        buttonReloadOrAbort->setText(QApplication::translate("MainWindow2", "Reload", 0, QApplication::UnicodeUTF8));
        buttonSelect_all->setText(QApplication::translate("MainWindow2", "Select all", 0, QApplication::UnicodeUTF8));
        buttonInverse->setText(QApplication::translate("MainWindow2", "Invert", 0, QApplication::UnicodeUTF8));
        buttonProcessor->setText(QApplication::translate("MainWindow2", "Processor", 0, QApplication::UnicodeUTF8));
        buttonCDDB->setText(QApplication::translate("MainWindow2", "CDDB", 0, QApplication::UnicodeUTF8));
        buttonMenu->setText(QApplication::translate("MainWindow2", "Menu", 0, QApplication::UnicodeUTF8));
        menuCoquillo->setTitle(QApplication::translate("MainWindow2", "Coquillo", 0, QApplication::UnicodeUTF8));
        menuSettings->setTitle(QApplication::translate("MainWindow2", "Settings", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("MainWindow2", "Help", 0, QApplication::UnicodeUTF8));
        dockEditor->setWindowTitle(QApplication::translate("MainWindow2", "Tags", 0, QApplication::UnicodeUTF8));
        dockLocation2->setWindowTitle(QApplication::translate("MainWindow2", "Location", 0, QApplication::UnicodeUTF8));
        mainToolBar->setWindowTitle(QApplication::translate("MainWindow2", "Main Toolbar", 0, QApplication::UnicodeUTF8));
        dockLocation->setWindowTitle(QApplication::translate("MainWindow2", "Path", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow2: public Ui_MainWindow2 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW2_H
