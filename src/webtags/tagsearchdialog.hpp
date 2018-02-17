#ifndef COQUILLO_WEBTAGS_TAGSEARCHDIALOG_H
#define COQUILLO_WEBTAGS_TAGSEARCHDIALOG_H

#include <QDialog>
#include <QModelIndex>
#include <QPointer>
#include <QVariantMap>

class QAbstractItemModel;
class QItemSelectionModel;

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
                TagSearchDialog(QItemSelectionModel * selection, QWidget * parent = nullptr);
                ~TagSearchDialog();

                QVariantMap resultData() const;
                void search(const QVariantMap & values);

            private slots:
                void on_buttonSearch_clicked();
                void showSearchResult(const QModelIndex & idx);

            private:
                Ui::TagSearchDialog * _ui;
                VariantHashModel * _results;
                AlbumDetailsModel * _details;

                QVariantMap _lastResult;
        };
    }
}

#endif
