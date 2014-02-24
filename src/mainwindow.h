#ifndef COQUILLO_MAINWINDOW_H
#define COQUILLO_MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
};

namespace Coquillo {
    class FileBrowser;
    class MetaDataModel;

    namespace Processor {
        class RenameWidget;
    }

    namespace TagEditor {
        class BasicTags;
    }

    class MainWindow : public QMainWindow {
        Q_OBJECT

        public:
            MainWindow(QWidget * parent=0);
            ~MainWindow();

        protected:
            void closeEvent(QCloseEvent * event);

        private slots:
            void applyValue(const QVariant & value, int column);
            void invertSelection();
            void restoreSettings();
            void selectAll();
            void showHeaderContextMenu(const QPoint & point) const;

        private:
            void saveSettings();

            Ui::MainWindow * _ui;
            FileBrowser * _fileBrowser;
            MetaDataModel * _metaData;
            TagEditor::BasicTags * _basicTags;
            Processor::RenameWidget * _fileRenamer;
    };
}

#endif
