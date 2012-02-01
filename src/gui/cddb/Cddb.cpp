#include "Cddb.h"

Cddb::Cddb(QObject * parent) : QObject(parent) {
	d = new CddbPrivate(this);

	connect(d, SIGNAL(queryError(int, const QString &)),
		SIGNAL(queryError(int, const QString &)));

	connect(d, SIGNAL(albumSearchFinished(const CddbAlbumInfoList &, int, int)),
		SIGNAL(albumSearchFinished(const CddbAlbumInfoList &, int, int)));

	connect(d, SIGNAL(trackListingFinished(const CddbTrackListing &)),
		SIGNAL(trackListingFinished(const CddbTrackListing &)));

}

void Cddb::sendSearchQuery(const QString & words, CddbFields fields, CddbCategories cats) {
	d->searchQuery(words, fields, cats);
}

void Cddb::sendAlbumSearchQuery(const QString & discId, const QList<quint32> & offsets, int playTime) {
	d->albumSearchQuery(discId, offsets, playTime);
}

void Cddb::sendTrackListingQuery(const QString & discId, const QString & category) {
	d->trackListingQuery(discId, category);
}
