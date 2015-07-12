#ifndef COQUILLO_TAGEDITOR_BASICTAGS_H
#define COQUILLO_TAGEDITOR_BASICTAGS_H

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
                QAbstractItemModel * model() const;

            public slots:
                void setEditorIndex(const QModelIndex & idx);

            signals:
                void cloneField(int column, const QVariant & value);

            private slots:
                void emitCloneField(int column);

            private:
                QDataWidgetMapper * _inputMapper;
                QDataWidgetMapper * _labelMapper;
                Ui::BasicTags * _ui;
        };
    };
};

#endif
