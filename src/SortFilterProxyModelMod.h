#ifndef SORTFILTERPROXYMODELMOD_H
#define SORTFILTERPROXYMODELMOD_H

#include <QSortFilterProxyModel>

// This model will sort path column according to the depth of directory.
class SortFilterProxyModelMod : public QSortFilterProxyModel {
	public:
		SortFilterProxyModelMod(QObject * p=0) : QSortFilterProxyModel(p) { }

	protected:
		bool lessThan(const QModelIndex & left, const QModelIndex & right) const;
};

#endif
