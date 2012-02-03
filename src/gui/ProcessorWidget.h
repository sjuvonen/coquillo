#ifndef PROCESSORWIDGET_H
#define PROCESSORWIDGET_H

#include <core/MetaData.h>

#include "DataWidget.h"

namespace Ui {
	class ProcessorWidget;
}

class QComboBox;

class ProcessorWidget : public DataWidget {
	Q_OBJECT

	public:
		ProcessorWidget(QWidget * parent=0);
		~ProcessorWidget();

		bool eventFilter(QObject * object, QEvent * event);

	public slots:
		void setSelection(const QItemSelection & selection);

	protected:
		void hideEvent(QHideEvent * event);
		void keyReleaseEvent(QKeyEvent * event);

	private slots:
		void apply();
		void updatePreviews();

	private:
		enum Action { RenameFiles, ResolveTags, ProcessFields };
		enum PatternHistory { RenameHistory, ResolveHistory };

		QStringList extractSymbols(const QString & pattern, bool unique=false) const;
		QString pathForPattern(const QString & pattern, int row, bool fullPath=true) const;
		MetaData tagsForPattern(QString pattern, int row) const;

		void processFields() const;
		void readPatternHistory(QStringList * list, int type) const;
		void savePatternHistory(const QStringList & list, int type) const;
		void saveSettings();
		void setHistory(const QStringList & list, QComboBox * widget);

		void updateRenamePreview();
		void updateResolvePreview();
		
		Ui::ProcessorWidget * _ui;

		QHash<QString, int> _symbols;
		QStringList _resolveHistory;
		QStringList _renameHistory;
};

#endif