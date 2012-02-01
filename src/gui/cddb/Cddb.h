#ifndef CDDB_H
#define CDDB_H

#include "CddbPrivate.h"

typedef int CddbFields;
typedef int CddbCategories;

class Cddb : public QObject {
	Q_OBJECT

	public:
		Cddb(QObject * parent=0);

		enum CddbField { AllFields=0, ArtistField=1, AlbumField=2, TitleField=4 };
		enum CddbCategory { AllCategories=0 };

		void sendSearchQuery(const QString & words, CddbFields fields=AllFields, CddbCategories=AllCategories);
		void sendAlbumSearchQuery(const QString & discId, const QList<quint32> & offsets, int playTime);
		void sendTrackListingQuery(const QString & discId, const QString & category);

		void setHost(const QString & h) { d->host = h; }
		QString host() const { return d->host; }

		void setPath(const QString & p) { d->path = p; }
		QString path() { return d->path; }

		void setPort(int p) { d->port = p; }
		int port() const { return d->port; }

		void setLocalHost(const QString & h) { d->localhost = h; }
		QString localHost() const { return d->localhost; }

		void setUserName(const QString & u) { d->username = u; }
		QString userName() const { return d->username; }

	signals:
		void queryError(int code, const QString & message);
		void albumSearchFinished(const CddbAlbumInfoList & matches, int, int);
		void trackListingFinished(const CddbTrackListing & list);

	private:
		CddbPrivate * d;

};

#endif