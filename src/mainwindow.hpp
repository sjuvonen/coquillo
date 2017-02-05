#ifndef COQUILLO_MAINWINDOW_H
#define COQUILLO_MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

namespace Coquillo {
    namespace Processor {
        class ParserWidget;
        class RenameWidget;
    }

    namespace Tags {
        class TagsModel;
    }

    class FileBrowser;
    class ProgressListener;
    class SortPicker;

    class MainWindow : public QMainWindow {
        Q_OBJECT

        public:
            MainWindow(Qt::WindowFlags flags = Qt::WindowFlags());
            ~MainWindow();
            QMenu * createPopupMenu();

        public slots:
            void addPaths(const QStringList & paths);
            void sort(int column, Qt::SortOrder = Qt::AscendingOrder);

        protected:
            void closeEvent(QCloseEvent * event);

        private slots:
            void onSortComboValueChanged(int value);
            void openSettingsDialog();
            void selectAll();
            void setInterfaceLocked(bool state);
            void showHeaderContextMenu(const QPoint & point) const;

        private:
            void applyDefaultSettings();
            void setupFileBrowser();
            void setupMainView();
            void setupParserWidget();
            void setupRenameWidget();
            void setupStatusBar();
            void setupTagEditor();
            void setupToolBar();
            void restoreSettings();
            void saveSettings();

            FileBrowser * _files;
            Tags::TagsModel * _model;
            Processor::ParserWidget * _tag_parser;
            Processor::RenameWidget * _file_rename;
            Ui::MainWindow * _ui;
            ProgressListener * _progress;
            SortPicker * _sort_picker;
    };
}

#endif
