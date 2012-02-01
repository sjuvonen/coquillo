#ifndef CDDBSEARCHDIALOG_H
#define CDDBSEARCHDIALOG_H

#include <QModelIndex>

#include "Cddb.h"
#include "DataWidget.h"

namespace Ui {
	class CddbSearchDialog;
}

class QAbstractButton;
class QStandardItem;
class QStandardItemModel;

#define CddbCacheRole Qt::UserRole+1

class CddbSearchDialog : public DataWidget {
	Q_OBJECT

	public:
		CddbSearchDialog(QWidget * parent=0);
		~CddbSearchDialog();

		void setModel(QAbstractItemModel * model) { _dataModel = model; }
		QAbstractItemModel * model() const { return _dataModel; }

	public slots:
		void accept();
		void clearCddbData();
		void setSelection(const QItemSelection &);

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

