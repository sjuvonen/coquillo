#include "filebrowser.h"
#include "filebrowsermodel.h"
#include "ui_filebrowser.h"
#include <qcheckbox.h>

namespace Coquillo {
FileBrowser::FileBrowser(QWidget *parent)
    : QWidget(parent), ui(new Ui::FileBrowser), fs(new FileBrowserModel) {
    ui->setupUi(this);
    ui->directoryView->setModel(fs);

    connect(ui->toggleRecursiveScan, &QCheckBox::toggled, fs, &FileBrowserModel::setRecursive);

    connect(ui->toggleRecursiveScan, &QCheckBox::toggled, this, &FileBrowser::recursiveToggled);

    connect(fs, &FileBrowserModel::pathAdded,
            [=](auto const &path) { emit pathAdded(path, ui->toggleRecursiveScan->checkState()); });

    connect(fs, &FileBrowserModel::pathRemoved, [=](auto const &path) {
        emit pathRemoved(path, ui->toggleRecursiveScan->checkState());
    });
}

FileBrowser::~FileBrowser() { delete ui; }

void FileBrowser::setDirectory(const QString &directory) {
    if (QFileInfo(directory).isDir()) {
        fs->setDirectory(directory);

        ui->workingDirectory->setCurrentText(directory);
        ui->directoryView->setRootIndex(fs->directoryIndex());
    }
}

void FileBrowser::setRecursive(bool recursive) { fs->setRecursive(recursive); }

} // namespace Coquillo
