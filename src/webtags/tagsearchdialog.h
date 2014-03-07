#ifndef COQUILLO_WEBTAGS_TAGSEARCHDIALOG_H
#define COQUILLO_WEBTAGS_TAGSEARCHDIALOG_H

#include <QDialog>
#include <QPointer>

class QAbstractItemModel;

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
                void setModel(QAbstractItemModel * model);
                QAbstractItemModel * model() const;

            private slots:
                void moveCurrentDown();
                void moveCurrentUp();
                void selectCurrent();
                void unselectCurrent();

            private:
                void selectPaths(const QStringList & paths);
                void unselectPaths(const QStringList & paths);
                Ui::TagSearchDialog * _ui;
                QPointer<QAbstractItemModel> _model;
        };
    }
}

#endif
