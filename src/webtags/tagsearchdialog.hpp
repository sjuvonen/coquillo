#ifndef COQUILLO_WEBTAGS_TAGSEARCHDIALOG_H
#define COQUILLO_WEBTAGS_TAGSEARCHDIALOG_H

#include <QDialog>
#include <QPointer>

class QAbstractItemModel;

namespace Ui {
    class TagSearchDialog;
}

namespace Coquillo {
    class VariantHashModel;

    namespace WebTags {
        class AlbumDetailsModel;

        class TagSearchDialog : public QDialog {
            Q_OBJECT

            public:
                TagSearchDialog(QWidget * parent = nullptr);
                ~TagSearchDialog();

                void setModel(QAbstractItemModel * model);
                QAbstractItemModel * model() const;

                void setSelectedRows(const QList<QModelIndex> & rows);

                QList<QVariantHash> resultData() const;
                void search(const QVariantMap & values);

            private slots:
                void on_buttonSearch_clicked();
                void showSearchResult(const QModelIndex & idx);

            private:
                Ui::TagSearchDialog * _ui;
                QPointer<QAbstractItemModel> _model;
                VariantHashModel * _results;
                AlbumDetailsModel * _details;
        };
    }
}

#endif
