#ifndef COQUILLO_MAINWINDOW_H
#define COQUILLO_MAINWINDOW_H

#include "mediastorage.h"
#include "mediastoragemodel.h"
#include "settings.h"
#include <QMainWindow>
#include <QPointer>

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

  protected:
    void resizeEvent(QResizeEvent *event);

  private:
    Ui::MainWindow *ui;
    QPointer<Settings> settings;
    QPointer<MediaStorage> storage;
    QPointer<MediaStorageModel> model;

    void setup();
};
} // namespace Coquillo
#endif
