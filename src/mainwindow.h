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
        class ParserWidget;
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
            QMenu * createPopupMenu();

        protected:
            void closeEvent(QCloseEvent * event);

        private slots:
            void applyValue(const QVariant & value, int column);
            void invertSelection();
            void restoreSettings();
            void selectAll();
            void setToolBarIconSize(int size);
            void setToolBarIconStyle(int style);
            void showHeaderContextMenu(const QPoint & point) const;

            void debug(QObject * obj = 0);
            void debug(QAction * obj);

        private:
            void saveSettings();

            Ui::MainWindow * _ui;
            FileBrowser * _fileBrowser;
            MetaDataModel * _metaData;
            TagEditor::BasicTags * _basicTags;
            Processor::ParserWidget * _tagParser;
            Processor::RenameWidget * _fileRenamer;
    };
}

#endif
