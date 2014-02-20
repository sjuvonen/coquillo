#ifndef COQUILLO_BASICTAGS_H
#define COQUILLO_BASICTAGS_H

#include <QWidget>

class QAbstractItemModel;
class QDataWidgetMapper;

namespace Ui {
    class BasicTags;
};

namespace Coquillo {
    namespace TagEditor {
        class BasicTags : public QWidget {
            Q_OBJECT

            public:
                BasicTags(QWidget * parent = 0);
                ~BasicTags();

                void setModel(QAbstractItemModel * model);
                inline QAbstractItemModel * model() const;

            public slots:
                void setCurrentIndex(const QModelIndex & idx);

            signals:
                void cloneValue(const QVariant & value, int column);

            private slots:
                void emitCloneValue(int column);

            private:
                QDataWidgetMapper * _inputMapper;
                QDataWidgetMapper * _labelMapper;
                Ui::BasicTags * _ui;
        };
    };
};

#endif
