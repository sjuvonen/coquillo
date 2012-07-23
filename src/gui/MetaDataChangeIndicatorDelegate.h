#ifndef METADATACHANGEINDICATORDELEGATE_H
#define METADATACHANGEINDICATORDELEGATE_H

#include <QStyledItemDelegate>

class MetaDataChangeIndicatorDelegate : public QStyledItemDelegate {
	Q_OBJECT

	public:
		MetaDataChangeIndicatorDelegate(QObject * parent = 0);

		void setEditorData(QWidget * editor, const QModelIndex & idx) const;
};

#endif
