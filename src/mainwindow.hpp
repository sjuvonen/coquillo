#ifndef COQUILLO_MAINWINDOW_H
#define COQUILLO_MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

namespace Coquillo {
    class MainWindow : public QMainWindow {
        Q_OBJECT

        public:
            MainWindow(Qt::WindowFlags flags = Qt::WindowFlags());
            ~MainWindow();

        private slots:
            void showHeaderContextMenu(const QPoint & point) const;

        private:
            Ui::MainWindow * _ui;
    };
}

#endif
