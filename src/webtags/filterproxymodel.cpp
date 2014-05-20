
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
            const QString path = idx.data(MetaData::MetaDataModel::FilePathRole).toString();
            return _filtered.contains(path) ^ (_mode == HideFiltered);
        }

        void FilterProxyModel::setFilterMode(FilterMode mode) {
            _mode = mode;
            invalidateFilter();
        }

//         QVariant FilterProxyModel::headerData(int section, Qt::Orientation orientation, int role) const {
// //             qDebug() << "here 1" << role;
//             if (section == Qt::Vertical && role == Qt::DecorationRole) {
// //                 qDebug() << "here 2";
// //                 return 4;
//                 qDebug() << section;
//                 return 555;
//                 return section + 1;
//             } else {
//                 return QSortFilterProxyModel::headerData(section, orientation, role);
//             }
//         }
    }
}
