#ifndef COQUILLO_MAINWINDOW_H
#define COQUILLO_MAINWINDOW_H

#include "mediastorage.h"
#include <QMainWindow>
#include <QPointer>
#include <QProgressBar>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace Coquillo {
class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private:
    Ui::MainWindow *ui;
    QPointer<QProgressBar> progressBar;
    QPointer<MediaStorage> storage;

    void setup();
};
} // namespace Coquillo
#endif
