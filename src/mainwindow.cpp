#include <QSignalMapper>

#include "tags/tagsmodel.hpp"
#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QDebug>

namespace Coquillo {
    MainWindow::MainWindow(Qt::WindowFlags flags)
    : QMainWindow(0, flags) {
        _ui = new Ui::MainWindow;
        _ui->setupUi(this);

        auto model = new Tags::TagsModel(this);
        _ui->tableView->setModel(model);
        _ui->tableView->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);

        connect(_ui->tableView->horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(showHeaderContextMenu(QPoint)));
    }

    MainWindow::~MainWindow() {
        delete _ui;
    }

    void MainWindow::showHeaderContextMenu(const QPoint & point) const {
        QHeaderView * header = _ui->tableView->horizontalHeader();
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
}
