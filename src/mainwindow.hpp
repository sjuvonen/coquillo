#ifndef COQUILLO_MAINWINDOW_H
#define COQUILLO_MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

namespace Coquillo {
    namespace Tags {
        class TagsModel;
    }

    class FileBrowser;
    class ProgressListener;

    class MainWindow : public QMainWindow {
        Q_OBJECT

        public:
            MainWindow(Qt::WindowFlags flags = Qt::WindowFlags());
            ~MainWindow();
            QMenu * createPopupMenu();

        protected:
            void closeEvent(QCloseEvent * event);

        private slots:
            void openSettingsDialog();
            void setInterfaceLocked(bool state);
            void showHeaderContextMenu(const QPoint & point) const;

        private:
            void setupFileBrowser();
            void setupMainView();
            void setupStatusBar();
            void setupTagEditor();
            void setupToolBar();
            void restoreSettings();
            void saveSettings();

            FileBrowser * _files;
            Tags::TagsModel * _model;
            Ui::MainWindow * _ui;
            ProgressListener * _progress;
    };
}

#endif
