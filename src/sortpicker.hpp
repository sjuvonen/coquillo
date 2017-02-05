#ifndef COQUILLO_SORTPICKER_H
#define COQUILLO_SORTPICKER_H

#include <QWidget>

class QAbstractItemModel;
class QSortFilterProxyModel;

namespace Ui {
    class SortPicker;
}

namespace Coquillo {
    class SortPicker : public QWidget {
        Q_OBJECT

        public:
            SortPicker(QWidget * parent = nullptr);
            void setModel(QAbstractItemModel * model);
            QAbstractItemModel * model() const;

            int currentIndex() const;

        public slots:
            void setCurrentIndex(int index);
            void onComboBoxIndexChanged(int index);

        signals:
            void currentIndexChanged(int index);

        private slots:

        private:
            Ui::SortPicker * _ui;
            QSortFilterProxyModel * _proxy;
    };
}

#endif
