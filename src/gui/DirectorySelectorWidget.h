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

		bool eventFilter(QObject * object, QEvent * event);

	signals:
		void bookmarked(const QString &);
		void pathChanged(const QString &);

	public slots:

		/**
		 * Changes visual root to current index's parent.
		 *
		 * @see setRootIndex(const QModelIndex &)
		 **/
		void goDirectoryUp();

		/**
		 * Sets the selected path and emits pathSelected(). (Or will it?)
		 *
		 * Will also set this path as visual root IF this directory is not a child
		 * for the current root.
		 **/
		void setPath(const QString &);

		/**
		 * Sets the visual root for the tree.
		 *
		 * Emits pathChanged() ONLY if current index is not a subdirectory of
		 * this directory.
		 **/
		void setRootIndex(const QModelIndex &);

		/**
		 * @see setRootIndex(const QModelIndex &)
		 **/
		void setRootPath(const QString &);

	private slots:
		void changePath(const QModelIndex &);
		void changeToTypedPath();

		void showTreeContextMenu(const QPoint &);

	private:
		Ui::DirectorySelector * _ui;
		QFileSystemModel * _model;
		FileSystemProxyModel * _proxy;
};

#endif
