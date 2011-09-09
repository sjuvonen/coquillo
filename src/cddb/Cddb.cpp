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
