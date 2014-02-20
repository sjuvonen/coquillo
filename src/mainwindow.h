#ifndef COQUILLO_MAINWINDOW_H
#define COQUILLO_MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
};

namespace Coquillo {
    class FileBrowser;
    class MetaDataModel;

    namespace TagEditor {
        class BasicTags;
    }

    class MainWindow : public QMainWindow {
        Q_OBJECT

        public:
            MainWindow(QWidget * parent=0);
            ~MainWindow();

        private slots:
            void applyValue(const QVariant & value, int column);

        private:
            Ui::MainWindow * _ui;
            FileBrowser * _fileBrowser;
            MetaDataModel * _metaData;
            TagEditor::BasicTags * _basicTags;
    };
}

#endif
