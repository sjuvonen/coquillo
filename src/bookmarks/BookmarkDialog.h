#ifndef BOOKMARKDIALOG_H
#define BOOKMARKDIALOG_H

#include <QDialog>

namespace Ui {
	class BookmarkDialog;
};

class BookmarkModel;

class BookmarkDialog: public QDialog {
	Q_OBJECT

	public:
		BookmarkDialog(QWidget * parent=0);

		void setModel(BookmarkModel *);
		BookmarkModel * model() const;

	private slots:
		void addBookmark();
		void removeBookmark();
		void showBookmark(int);

	private:
		Ui::BookmarkDialog * _ui;
};

#endif