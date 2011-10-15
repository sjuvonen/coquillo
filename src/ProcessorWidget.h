/***********************************************************************
* Copyright (c) 2011 Samu Juvonen <samu.juvonen@gmail.com>
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
************************************************************************/

#ifndef FORMATTERWIDGET_H
#define FORMATTERWIDGET_H

#include <QPersistentModelIndex>
#include <QWidget>

#include "globals.h"

class QAbstractItemModel;

namespace Ui {
	class ProcessorWidget;
}

class ProcessorWidget : public QWidget {
	Q_OBJECT

	public:
		enum Actions { ResolveTagsFromFilenames, RenameFilesAndFolders, ProcessFields };

		ProcessorWidget(QWidget * parent=0);
		~ProcessorWidget();

		void setModel(QAbstractItemModel * m) { _model = m; }

	signals:
		void closeButtonPressed();

	public slots:
		void setDataIndexes(const QModelIndexList &);
		void setFloatingLayout(bool setFloating);

	private slots:
		void apply();
		void updateRenamePreview();
		void updateResolvePreview();

	private:
		QStringList extractSymbols(const QString & pattern, bool unique=false) const;
		QString fileNameForPattern(const QString & pattern, const MetaData & metaData, bool fullPath=true) const;
		MetaData tagsFromFileName(const QString & pattern, const QString & fileName) const;

		MetaData metaData(const QModelIndex & idx) const;

		void processFields() const;
		void readPatternHistory(QStringList * history, int type) const;
		void savePatternHistory(const QStringList & history, int type) const;

		QAbstractItemModel * _model;
		QList<QPersistentModelIndex> _indexes;

		QMap<QString, QString> _symbols;
		QStringList _resolveHistory;
		QStringList _renameHistory;

		Ui::ProcessorWidget * _ui;

};

#endif
