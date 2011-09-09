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