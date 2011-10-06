
#include "BookmarkDialog.h"
#include "BookmarkModel.h"

#include "ui_BookmarkDialog.h"

BookmarkDialog::BookmarkDialog(QWidget * parent) : QDialog(parent) {
	_ui = new Ui::BookmarkDialog;
	_ui->setupUi(this);

	connect(_ui->bookmarks, SIGNAL(currentIndexChanged(int)), SLOT(showBookmark(int)));
}

void BookmarkDialog::setModel(BookmarkModel * model) {
	_ui->bookmarks->setModel(model);
}

BookmarkModel * BookmarkDialog::model() const {
	return qobject_cast<BookmarkModel *>(_ui->bookmarks->model());
}



void BookmarkDialog::addBookmark() {

	if (_ui->location->text().isEmpty())
		return;
	
	if (model())
		model()->addPath(_ui->location->text(), _ui->name->text());
}

void BookmarkDialog::removeBookmark() {
	if (!model())
		return;

	model()->removeRow( _ui->bookmarks->currentIndex() );
}

void BookmarkDialog::showBookmark(int row) {
	if (!model())
		return;
	
	const QModelIndex idx = model()->index(row, 0);

	_ui->location->setText(idx.data(BookmarkModel::FilePathRole).toString());
	_ui->name->setText(idx.data(BookmarkModel::DisplayNameRole).toString());
}