#ifndef COQUILLO_MAINWINDOW_H
#define COQUILLO_MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
};

namespace Coquillo {
    class FileBrowser;

    namespace MetaData {
        class MetaDataModel;
    }

    namespace Processor {
        class ParserWidget;
        class RenameWidget;
    }

    namespace TagEditor {
        class BasicTags;
        class RawData;
    }

    class MainWindow : public QMainWindow {
        Q_OBJECT

        public:
            MainWindow(QWidget * parent=0);
            ~MainWindow();
            QMenu * createPopupMenu();

        protected:
            void closeEvent(QCloseEvent * event);
            bool isInterfaceLocked() const;

        private slots:
            void applyValue(const QVariant & value, int column);
            void invertSelection();
            void lockInterface(bool state);
            void openSettingsDialog();
            void openTagSearchDialog();
            void restoreSettings();
            void selectAll();
            void setToolBarIconSize(int size);
            void setToolBarIconStyle(int style);
            void showHeaderContextMenu(const QPoint & point) const;

        private:
            void saveSettings();
            void setupTagEditor();

            Ui::MainWindow * _ui;
            FileBrowser * _fileBrowser;
            MetaData::MetaDataModel * _metaData;
            TagEditor::BasicTags * _basicTags;
            TagEditor::RawData * _rawTags;
            Processor::ParserWidget * _tagParser;
            Processor::RenameWidget * _fileRenamer;
    };
}

#endif
