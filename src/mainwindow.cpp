#include <QSortFilterProxyModel>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "filebrowser/filebrowser.h"
#include "metadata/metadatamodel.h"
#include "tageditor/basictags.h"

namespace Coquillo {
    MainWindow::MainWindow(QWidget * parent)
    : QMainWindow(parent) {
        _ui = new Ui::MainWindow;
        _ui->setupUi(this);

        _metaData = new MetaDataModel(this);

        QSortFilterProxyModel * sort_proxy = new QSortFilterProxyModel(this);
        sort_proxy->setSourceModel(_metaData);
        _ui->metaData->setModel(sort_proxy);

        _fileBrowser = new FileBrowser(this);
        _ui->tools->addTab(_fileBrowser, tr("Directories"));

        _basicTags = new TagEditor::BasicTags(this);
        _basicTags->setModel(_ui->metaData->model());
        _ui->tagEditor->addTab(_basicTags, tr("Tags"));

        connect(_fileBrowser, SIGNAL(recursionEnabled(bool)),
            _metaData, SLOT(setRecursive(bool)));

        connect(_fileBrowser, SIGNAL(pathSelected(QString, bool)),
            _metaData, SLOT(addDirectory(QString)));

        connect(_fileBrowser, SIGNAL(pathUnselected(QString, bool)),
            _metaData, SLOT(removeDirectory(QString)));

        connect(_ui->metaData->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
            _basicTags, SLOT(setCurrentIndex(QModelIndex)));

        connect(_basicTags, SIGNAL(cloneValue(QVariant, int)), SLOT(applyValue(QVariant, int)));

        connect(_ui->actionReset, SIGNAL(triggered()), _metaData, SLOT(revert()));

        _ui->metaData->header()->setSectionResizeMode(0, QHeaderView::Fixed);
        _ui->metaData->header()->resizeSection(0, 20);
        _ui->metaData->header()->setSortIndicator(1, Qt::AscendingOrder);
    }

    MainWindow::~MainWindow() {
        delete _ui;
    }

    void MainWindow::applyValue(const QVariant & value, int column) {
        foreach (QModelIndex idx, _ui->metaData->selectionModel()->selectedRows(column)) {
            const_cast<QAbstractItemModel*>(idx.model())->setData(idx, value);
        }
    }
}
