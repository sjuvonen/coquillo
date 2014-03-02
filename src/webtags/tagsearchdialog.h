#ifndef COQUILLO_WEBTAGS_TAGSEARCHDIALOG_H
#define COQUILLO_WEBTAGS_TAGSEARCHDIALOG_H

#include <QDialog>

namespace Ui {
    class TagSearchDialog;
}

namespace Coquillo {
    namespace WebTags {
        class TagSearchDialog : public QDialog {
            Q_OBJECT

            public:
                TagSearchDialog(QWidget * parent = 0);
                ~TagSearchDialog();

            private:
                Ui::TagSearchDialog * _ui;
        };
    }
}

#endif
