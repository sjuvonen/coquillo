#include <QDebug>

#include "MetaDataChangeIndicatorDelegate.h"
#include <core/MetaDataModel2.h>

typedef MetaDataModel2 MetaDataModel;

MetaDataChangeIndicatorDelegate::MetaDataChangeIndicatorDelegate(QObject * parent)
	: QStyledItemDelegate(parent) {

}

void MetaDataChangeIndicatorDelegate::setEditorData(QWidget * editor, const QModelIndex & idx) const {
	if (!editor || !editor->inherits("QLabel")) {
		return;
	}

	/*
	 * Set bold font to labels whose corresponding field has changed from the
	 * original value.
	 */

	QFont font = editor->font();
	font.setBold(idx.data(MetaDataModel::FieldChangedRole).toBool());

	editor->setFont(font);
}
