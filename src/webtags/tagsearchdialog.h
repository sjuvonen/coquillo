#ifndef COQUILLO_WEBTAGS_TAGSEARCHDIALOG_H
#define COQUILLO_WEBTAGS_TAGSEARCHDIALOG_H

#include <QDialog>
#include <QMap>
#include <QPointer>
#include <QVariant>

class QAbstractItemModel;

namespace Ui {
    class TagSearchDialog;
}

namespace Coquillo {
    namespace Searcher {
        class AbstractSearcher;
    }

    namespace WebTags {
        class TagSearchDialog : public QDialog {
            Q_OBJECT

            public:
                TagSearchDialog(QWidget * parent = 0);
                ~TagSearchDialog();
                void setModel(QAbstractItemModel * model);
                QAbstractItemModel * model() const;
                void addSearcher(Searcher::AbstractSearcher * searcher);

            private slots:
                void executeSearch();
                void executeFetchAlbum(const QModelIndex & idx);
                void moveCurrentDown();
                void moveCurrentUp();
                void reactToTabChange(int current);
                void selectCurrent();
                void showAlbumInfo(const QVariantMap & album);
                void showResults(const QList<QVariantMap> & results, const QString & source);
                void unselectCurrent();

            private:
                void search(const QVariantMap & params);
                void selectPaths(const QStringList & paths);
                void unselectPaths(const QStringList & paths);
                Ui::TagSearchDialog * _ui;
                QPointer<QAbstractItemModel> _model;
                QMap<QString, Searcher::AbstractSearcher*> _searchers;
        };
    }
}

#endif
