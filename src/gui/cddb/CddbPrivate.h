#ifndef CDDBPRIVATE_H
#define CDDBPRIVATE_H

#include <QObject>

#include <QMap>
#include <QObject>
#include <QStringList>

class QNetworkAccessManager;
class QNetworkReply;

// Contains information from operation 'cddb query'.
struct CddbAlbumInfo {
	QString title;
	QString category;
	QString discId;
};

struct CddbTrackListing {
	CddbAlbumInfo albumInfo;
	QStringList tracks;
	QString genre;
	int year;
};

typedef QList<CddbAlbumInfo> CddbAlbumInfoList;

class CddbPrivate : public QObject {
	Q_OBJECT

	public:
		CddbPrivate(QObject * parent=0);

		QNetworkAccessManager * manager;

		QString host;
		QString path;
		QString username;
		QString localhost;
		QString userAgent;
		QString agentVersion;

		int port;

		/**
		 * Run a custom search using form at www.freedb.org/freedb_search.php
		 **/
		void searchQuery(const QString & words, int fields, int cats);

		/**
		 * Query for CDDB information using the calculated disc ID
		 **/
		void albumSearchQuery(const QString & discId, const QList<quint32> & offsets, int playTime);

		/**
		 * Fetch track listing for a certain album
		 **/
		void trackListingQuery(const QString & discId, const QString & category);

	signals:
		void queryError(int code, const QString & message);
		void albumSearchFinished(const CddbAlbumInfoList & firstPage, int total, int pages);
		void trackListingFinished(const CddbTrackListing & list);

	private slots:
		void replyFinished(QNetworkReply *);

	private:
		/**
		 * Common string parts used in a query
		 **/
		QString address() const { return QString("%1:%2%3").arg(host).arg(port).arg(path); }
		QString hello() const { return QString("hello=%1+%2+%3+%4").arg(username, localhost, userAgent, agentVersion); }
		QString proto() const { return QString("proto=6"); }

		CddbTrackListing parseTrackListing(const QString & data) const;
		
		// Parses search results. Puts the number of results in variable 'total' and number of pages in 'pages'.
		QList<CddbAlbumInfo> parseSearchResult(const QString & data, int * total, int * pages) const;

		/**
		 * Each query is given a type so that we know how to process the fetched data.
		 **/
		enum QueryType { InvalidQuery, SearchQuery, AlbumSearchQuery, TrackListingQuery };
		QMap<QNetworkReply*, QueryType> queries;
};

#endif
