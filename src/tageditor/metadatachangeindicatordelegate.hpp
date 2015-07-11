#ifndef COQUILLO_METADATACHANGEINDICATORDELEGATE_H
#define COQUILLO_METADATACHANGEINDICATORDELEGATE_H

#include <QStyledItemDelegate>

namespace Coquillo {
    class MetaDataChangeIndicatorDelegate : public QStyledItemDelegate {
        Q_OBJECT

        public:
            MetaDataChangeIndicatorDelegate(QObject * parent = 0);
            void setEditorData(QWidget * editor, const QModelIndex & idx) const;
    };
}

#endif
