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
#include <qevent.h>

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

    QMenu *createPopupMenu() override;

  protected:
    bool event(QEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

  private:
    void lockToolBars(bool lock = true);
    void showHeaderContextMenu(const QPoint &pos);
    void setup();
    void restore();

    Ui::MainWindow *ui;
    QPointer<Settings> settings;
    QPointer<MediaStorage> storage;
    QPointer<MediaStorageModel> storageModel;
    QPointer<QSortFilterProxyModel> proxyModel;
    QPointer<SelectionNotifier> selectionNotifier;
    QPointer<CurrentMediaPathDelegate> currentMediaPath;
};
} // namespace Coquillo
#endif
