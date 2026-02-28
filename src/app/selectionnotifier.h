#ifndef COQUILLO_SELECTIONQUEUE_H
#define COQUILLO_SELECTIONQUEUE_H

#include <QAbstractItemModel>
#include <QItemSelectionModel>
#include <QObject>
#include <QPointer>

namespace Coquillo {
class SelectionNotifier : public QObject {
    Q_OBJECT

  public:
    SelectionNotifier(QObject *parent = nullptr);

    QAbstractItemModel *model();
    void setModel(QAbstractItemModel *model);

    void setSelectionModel(QItemSelectionModel *selectionModel);

    inline const QList<int> selection() const;
    void setSelection(const QList<int> &rows);

    inline int current() const;
    void setCurrent(int row);

    void previous();
    void next();

  signals:
    void currentChanged(int row);
    void selectionChanged();

  private:
    QPointer<QAbstractItemModel> _model;
    QPointer<QItemSelectionModel> _selectionModel;
    QList<int> _selection;
    int _current;
};

const QList<int> SelectionNotifier::selection() const { return _selection; }

int SelectionNotifier::current() const { return _current; }
}; // namespace Coquillo
#endif