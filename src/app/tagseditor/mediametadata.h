#ifndef COQUILLO_MEDIAMETADATA_H
#define COQUILLO_MEDIAMETADATA_H

#include "../selectionnotifier.h"
#include "mediastorage.h"
#include "metadatamodel.h"
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MediaMetadata;
}
QT_END_NAMESPACE

namespace Coquillo {

class MediaMetadata : public QWidget {
    Q_OBJECT

  public:
    MediaMetadata(QWidget *parent = nullptr);
    ~MediaMetadata();

    void setMediaStorage(MediaStorage *storage);
    void setSelectionNotifier(SelectionNotifier *selectionNotifier);

  private:
    Ui::MediaMetadata *ui;
    QPointer<MetadataModel> model;
    QPointer<MediaStorage> storage;
    QPointer<SelectionNotifier> selection;
};
} // namespace Coquillo

#endif
