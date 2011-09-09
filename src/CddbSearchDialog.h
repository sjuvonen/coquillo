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

#ifndef CDDBSEARCHDIALOG_H
#define CDDBSEARCHDIALOG_H

#include <QDialog>
#include <QModelIndex>

#include "cddb/Cddb.h"

namespace Ui {
	class CddbSearchDialog;
}

class QAbstractButton;
class QStandardItem;
class QStandardItemModel;

#define CddbCacheRole Qt::UserRole+1

class CddbSearchDialog : public QDialog {
	Q_OBJECT

	public:
		CddbSearchDialog(QWidget * parent=0);
		~CddbSearchDialog();

		void setModel(QAbstractItemModel * model) { _dataModel = model; }
		QAbstractItemModel * model() const { return _dataModel; }

	public slots:
		void accept();
		void clearCddbData();
		void setIndexes(const QModelIndexList &);

	private slots:
		void search();
		void getAlbumData(const QModelIndex &);
		void showError(int code, const QString & message);
		void showMatches(const CddbAlbumInfoList & firstPage, int total, int pages);
		void showTrackListing(const CddbTrackListing &);
		void toggleTagButtonExclusion(QAbstractButton *);
		void toggleSearchFieldButtonExclusion(QAbstractButton *);

	protected:
		void closeEvent(QCloseEvent *);

	private:
		bool guessMultiArtist(const CddbTrackListing &) const;
		void showBusyIndicator(bool state);

		QList<QPersistentModelIndex> _indexes;

		QList<quint32> _offsets;
		QString _discId;
		quint64 _totalTime;

		Ui::CddbSearchDialog * _ui;

		QAbstractItemModel * _dataModel;
		QStandardItemModel * _cddbModel;

		// Cache fetched track listings
		QHash<QString, CddbTrackListing> _cache;

		Cddb * _cddb;

};

#endif

