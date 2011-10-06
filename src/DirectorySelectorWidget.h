#ifndef DIRECTORYSELECTORWIDGET_H
#define DIRECTORYSELECTORWIDGET_H

#include <QWidget>

class QFileSystemModel;
class QLineEdit;
class QModelIndex;
class QTreeView;

class FileSystemProxyModel;

namespace Ui {
	class DirectorySelector;
};

class DirectorySelectorWidget : public QWidget {
	Q_OBJECT

	public:
		DirectorySelectorWidget(QWidget * parent=0);
		QString path() const;

	signals:
		void bookmarked(const QString &);
		void pathSelected(const QString &);

	public slots:
		void goDirectoryUp();
		void setPath(const QString &);
		void setRootPath(const QString &);
		void setRootIndex(const QModelIndex &);

	private slots:
		void changePath(const QModelIndex &);
		void changeToTypedPath();

		void showTreeContextMenu(const QPoint &);

	protected:
		bool eventFilter(QObject * object, QEvent * event);

	private:
		Ui::DirectorySelector * _ui;
		QFileSystemModel * _model;
		FileSystemProxyModel * _proxy;
};

#endif
