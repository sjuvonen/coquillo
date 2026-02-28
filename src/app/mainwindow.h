#ifndef COQUILLO_MAINWINDOW_H
#define COQUILLO_MAINWINDOW_H

#include "currentmediapathdelegate.h"
#include "mediastorage.h"
#include "mediastoragemodel.h"
#include "selectionnotifier.h"
#include "settings.h"
#include <QMainWindow>
#include <QPointer>
#include <QSortFilterProxyModel>

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
    bool event(QEvent *event) override;

  private:
    Ui::MainWindow *ui;
    QPointer<Settings> settings;
    QPointer<MediaStorage> storage;
    QPointer<MediaStorageModel> storageModel;
    QPointer<QSortFilterProxyModel> proxyModel;
    QPointer<SelectionNotifier> selectionNotifier;
    QPointer<CurrentMediaPathDelegate> currentMediaPath;

    void setup();
};
} // namespace Coquillo
#endif
