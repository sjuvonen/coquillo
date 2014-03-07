
#include <QDebug>

#include <metadata/metadatamodel.h>
#include "filterproxymodel.h"

namespace Coquillo {
    namespace WebTags {
        FilterProxyModel::FilterProxyModel(QObject * parent)
        : QSortFilterProxyModel(parent), _mode(HideFiltered) {

        }

        void FilterProxyModel::addFilter(const QString & value) {
            if (!_filtered.contains(value)) {
                _filtered << value;
                invalidateFilter();
            }
        }

        void FilterProxyModel::addFilters(const QStringList & filters) {
            _filtered << filters;
            _filtered.removeDuplicates();
            invalidateFilter();
        }

        void FilterProxyModel::removeFilter(const QString & value) {
            _filtered.removeAll(value);
            invalidateFilter();
        }

        bool FilterProxyModel::filterAcceptsRow(int row, const QModelIndex & parent) const {
            const QModelIndex idx = sourceModel()->index(row, 0, parent);
            const QString path = idx.data(MetaDataModel::FilePathRole).toString();
            return _filtered.contains(path) ^ (_mode == HideFiltered);
        }

        void FilterProxyModel::setFilterMode(FilterMode mode) {
            _mode = mode;
            invalidateFilter();
        }
    }
}
