#ifndef COQUILLO_SELECTIONPROXYMODEL_H
#define COQUILLO_SELECTIONPROXYMODEL_H

#include <QItemSelectionModel>
#include <QPointer>
#include <QSortFilterProxyModel>

namespace Coquillo {
    class SelectionProxyModel : public QSortFilterProxyModel {
        Q_OBJECT

        public:
            SelectionProxyModel(QObject * parent = nullptr) : QSortFilterProxyModel(parent) { }
            SelectionProxyModel(QItemSelectionModel * selection, QObject * parent = nullptr);

            void setSelectionModel(QItemSelectionModel * model);
            QItemSelectionModel * selectionModel() const { return _selection; }

        protected:
            bool filterAcceptsRow(int source_row, const QModelIndex & source_parent) const;

        private:
            QPointer<QItemSelectionModel> _selection;
    };
}

#endif
