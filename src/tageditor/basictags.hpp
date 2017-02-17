#ifndef COQUILLO_TAGEDITOR_BASICTAGS_H
#define COQUILLO_TAGEDITOR_BASICTAGS_H

#include "editorpagebase.hpp"

class QAbstractItemModel;
class QDataWidgetMapper;

namespace Ui {
    class BasicTags;
}

namespace Coquillo {
    namespace TagEditor {
        class BasicTags : public EditorPageBase {
            Q_OBJECT

            public:
                BasicTags(QWidget * parent = nullptr);
                ~BasicTags();

                void setModel(QAbstractItemModel * model);

            public slots:
                void setEditorIndex(const QModelIndex & idx);

            private slots:
                void autoNumberItems();
                void cloneField(int column);

            private:
                void setupMappings();

                Ui::BasicTags * _ui;
                QDataWidgetMapper * _inputMapper;
                QDataWidgetMapper * _labelMapper;
        };
    }
}

#endif
