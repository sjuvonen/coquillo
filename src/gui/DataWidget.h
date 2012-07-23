#ifndef DATAWIDGET_H
#define DATAWIDGET_H

#include <QPersistentModelIndex>
#include <QWidget>

class QAbstractItemModel;
class QItemSelection;

class DataWidget : public QWidget {
	Q_OBJECT

	public:
		DataWidget(QWidget * parent=0);
		~DataWidget();

		QList<QPersistentModelIndex> rows() const { return _rows; }

		QAbstractItemModel * model() const { return _model; }
		virtual void setModel(QAbstractItemModel * model);

	public slots:
		virtual void setSelection(const QItemSelection & selection);

	private:
		QAbstractItemModel * _model;
		QList<QPersistentModelIndex> _rows;
};

#endif
