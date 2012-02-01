#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include "CddbPrivate.h"
#include "Cddb.h"

#include <QDebug>

CddbPrivate::CddbPrivate(QObject * parent)
: QObject(parent), username("anonymous"), localhost("localhost"),
  userAgent("Coquillo"), agentVersion( QApplication::applicationVersion() ), port(80) {

	manager = new QNetworkAccessManager(this);

	connect(manager, SIGNAL(finished(QNetworkReply*)), SLOT(replyFinished(QNetworkReply*)));
}

void CddbPrivate::searchQuery(const QString & words, int fields, int cats) {
	const QString addr("http://www.freedb.org/freedb_search.php");
	QString f;
	QString c;
	QString w = QString(words).replace(' ', '+');

	if (!cats)
		f.append("allcats=YES");

	if (!fields)
		c.append("allfields=YES");
	else {
		c.append("allfields=NO");

		if (fields & Cddb::ArtistField)
			c.append("&fields[]=artist");

		if (fields & Cddb::AlbumField)
			c.append("&fields[]=album");

		if (fields & Cddb::TitleField)
			c.append("&fields[]=title");
	}

	const QString query = QString("%1?words=%2&%3&%4").arg(addr, w, f, c);

	qDebug() << "send query:" << query;

	QNetworkRequest rq(query);
	QNetworkReply * r = manager->get(rq);

	queries.insert(r, SearchQuery);
}

void CddbPrivate::albumSearchQuery(const QString & discId, const QList<quint32> & offsets, int playTime) {
	QString addr = address();
	QString proto = this->proto();
	QString hello = this->hello();
	QString cmd = QString("cmd=cddb+query+%1+%2").arg(discId).arg(offsets.count());

	foreach (int off, offsets)
		cmd.append(QString("+%1").arg(off));

	cmd.append(QString("+%1").arg(playTime));

	const QString query = QString("%1?%2&%3&%4").arg(addr, cmd, hello, proto);

	qDebug() << "send query:" << query;

	QNetworkRequest rq(query);
	QNetworkReply * r = manager->get(rq);

	queries.insert(r, AlbumSearchQuery);
}

void CddbPrivate::trackListingQuery(const QString & discId, const QString & category) {
	QString addr = address();
	QString cmd = QString("cmd=cddb+read+%1+%2").arg(category, discId);
	const QString query = QString("%1?%2&%3&%4").arg(addr, cmd, hello(), proto());

	qDebug() << "send query" << query;

	QNetworkRequest rq(query);
	QNetworkReply * r = manager->get(rq);

	queries.insert(r, TrackListingQuery);
}


void CddbPrivate::replyFinished(QNetworkReply * reply) {
	 QueryType type = queries.take(reply);

	 const QString data = reply->readLine();
	 int code = data.section(' ', 0, 0).toInt();

	 // Handle common errors first

	 bool error = false;

	 switch (code) {
		 case 402:
			emit queryError(code, "Server error");
			error = true;
			break;

		case 403:
			emit queryError(code, "Database entry corrupted");
			error = true;
			break;

		case 409:
			emit queryError(code, "No handshake");
			error = true;
			break;

		case 500:
			emit queryError(code, "Invalid number of arguments");
			error = true;
			break;
	 }

	 if (error) {
		 reply->deleteLater();
		 return;
	 }

	 switch (type) {
		 case InvalidQuery:
			 qDebug() << "Invalid query type for reply!";
			 break;

		 case AlbumSearchQuery:
			switch (code) {
				// Exact match
				case 200: {
					qDebug() << "album info" << data;
						CddbAlbumInfo info;
						info.category = data.section(' ', 1, 1);
						info.discId = data.section(' ', 2, 2);
						info.title = data.section(' ', 3, 3).trimmed();

						emit albumSearchFinished(QList<CddbAlbumInfo>() << info, 1, 1);
						break;
				 }

				// No match found
				case 202:
					emit albumSearchFinished(QList<CddbAlbumInfo>(), 0, 0);
					break;

				// Multiple matches
				case 211: {
					QList<CddbAlbumInfo> list;

					while (!reply->atEnd()) {
						const QString line = reply->readLine().trimmed();

						// Last row is termianting marker.
						if (reply->atEnd())
							break;

						CddbAlbumInfo info;
						info.category = line.section(' ', 0, 0);
						info.discId = line.section(' ', 1, 1);
						info.title = line.section(' ', 2);
						list << info;
					}

					emit albumSearchFinished(list, list.count(), 1);
					break;
				}
			 }

			 break;

		 case TrackListingQuery:
			 switch (code) {
				 // All OK, read listing
				 case 210: {
					 const CddbTrackListing list = parseTrackListing( data + reply->readAll());

					emit trackListingFinished(list);
				 }

				 default:
					;
			 }

			 break;

		 case SearchQuery: {
			 if (reply->error() != QNetworkReply::NoError) {
				 emit queryError(500, "Network error");
				 qDebug() << "Network error";
				 return;
			 }

				int total;
			 int pages;

			 CddbAlbumInfoList list = parseSearchResult( data + reply->readAll(), &total, &pages );

			 emit albumSearchFinished( list, total, pages );
			 break;
		 }

		 default:
			 qDebug() << "Unknown query:" << reply->readLine();
	 };

	 reply->deleteLater();
}

CddbTrackListing CddbPrivate::parseTrackListing(const QString & data) const {
	CddbTrackListing list;
	QStringList rows = data.split(QRegExp("(\r\n?)|(\r?\n)"));

	if (rows.count() == 0)
		return list;

	list.albumInfo.category = rows[0].section(' ', 1, 1);
	list.albumInfo.discId = rows[0].section(' ', 2, 2);

	for (int i = 1; i < rows.count(); i++) {
		const QString row = rows[i].trimmed();

		if (row.startsWith("DTITLE"))
			list.albumInfo.title = row.section('=', 1);
		else if (row.startsWith("DYEAR"))
			list.year = row.section('=', 1).toInt();
		else if (row.startsWith("DGENRE"))
			list.genre = row.section('=', 1);

		// Currently we hope that the tracks are sorted according to track number...
		else if (row.startsWith("TTITLE"))
			list.tracks << row.section('=', 1);

		else
			continue;
	}

	return list;
}

QList<CddbAlbumInfo> CddbPrivate::parseSearchResult(const QString & data, int * total, int * pages) const {
	// Parses albums from the result web page. Parse pretty much counts on that the page structure
	// will not be changed.

	QList<CddbAlbumInfo> list;

	// The first match is junk, begin from second
	int pos = data.indexOf("class=searchResultT1A");

	// Cut down the source for easier regex parsing
	const QString junk = data.mid(pos-100, 100);

	QRegExp rx("<br>(\\d+) result\\(s\\) found displayed on (\\d+) page\\(s\\).");

	rx.indexIn(junk);

	bool ok;

	*total = rx.cap(1).toInt(&ok);

	if (!ok)
		*total = -1;

	*pages = rx.cap(2).toInt(&ok);

	if (!ok)
		*pages = -1;

	while (pos != -1) {
		CddbAlbumInfo info;

		int titleEnd = data.indexOf("</a>", pos);
		int titleBegin = data.lastIndexOf(">", titleEnd) + 1;

		info.title = data.mid(titleBegin, titleEnd-titleBegin);

		int sub = data.indexOf("searchResultT2A", pos);

		int catBegin = data.indexOf("<b>Disc-ID:</b>", sub) + 15;
		int catEnd = data.indexOf(" / ", catBegin);

		info.category = data.mid(catBegin, catEnd-catBegin).trimmed();

		int idEnd = data.indexOf("</a>", catEnd);
		int idBegin = data.lastIndexOf(">", idEnd)+1;

		info.discId = data.mid(idBegin, idEnd-idBegin);

		list << info;

		pos = data.indexOf("class=searchResultT1A", pos+20);
	}

	return list;

}
