#ifndef COQUILLO_MAINWINDOW_H
#define COQUILLO_MAINWINDOW_H

#include <QMainWindow>

class QNetworkAccessManager;

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

    class MainWindow : public QMainWindow {
        Q_OBJECT

        public:
            MainWindow(QWidget * parent=0);
            ~MainWindow();
            QMenu * createPopupMenu();

            void openDirectories(const QStringList & dirs);

        protected:
            void closeEvent(QCloseEvent * event);
            bool isInterfaceLocked() const;

        private slots:
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

            Ui::MainWindow * _ui;
            FileBrowser * _fileBrowser;
            MetaData::MetaDataModel * _metaData;
            Processor::ParserWidget * _tagParser;
            Processor::RenameWidget * _fileRenamer;
            QNetworkAccessManager * _nam;
    };
}

#endif
