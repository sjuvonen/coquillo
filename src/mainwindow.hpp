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

    class MainWindow : public QMainWindow {
        Q_OBJECT

        public:
            MainWindow(Qt::WindowFlags flags = Qt::WindowFlags());
            ~MainWindow();

        protected:
            void closeEvent(QCloseEvent * event);

        private slots:
            void openSettingsDialog();
            void showHeaderContextMenu(const QPoint & point) const;

        private:
            void setupFileBrowser();
            void setupMainView();
            void setupTagEditor();
            void restoreSettings();
            void saveSettings();

            FileBrowser * _files;
            Tags::TagsModel * _model;
            Ui::MainWindow * _ui;
    };
}

#endif
