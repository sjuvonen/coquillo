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

#ifndef MEDIASCANNER_H
#define MEDIASCANNER_H

#include <QObject>

#include "def_MetaData.h"

class MediaScanner : public QObject {
	Q_OBJECT

	public:
		MediaScanner(QObject * parent=0);

		void setPath(const QString & p) { _path = p; }
		QString path() const { return _path; }

		void setRecursive(bool s) { _recursive = s; }
		bool isRecursive() const { return _recursive; }

	signals:
		void finished();
		void itemsDone(int);
		void itemFound(const MetaData &);
		void maxItemsChanged(int max);
		void started();

	public slots:
		void scan();
		void scanPath(const QString & path);

	private:
		QStringList validFileTypes() const;
		QStringList fetchMedia(const QString & path, const QStringList & fileTypes, bool recursive) const;

		void processMedia(const QStringList & paths);

		bool _recursive;
		QString _path;


};

#endif