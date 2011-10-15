
#include <metadata/MetaDataModel.h>

#include "SortFilterProxyModelMod.h"

#include <QDebug>

bool SortFilterProxyModelMod::lessThan(const QModelIndex & left, const QModelIndex & right) const {
	// Paths that have less depth, ie. less characters /, are 'less than'.

	if (left.column() == right.column() && left.column() == modelColumn("Path")) {
		const QString lp = left.data(Qt::EditRole).toString();
		const QString rp = right.data(Qt::EditRole).toString();
		int level = lp.count('/') - rp.count('/');

		return level == 0 ? lp < rp : level < 0;
	}

	return QSortFilterProxyModel::lessThan(left, right);
}
