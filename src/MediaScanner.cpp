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

#include <taglib/fileref.h>

#include <QDir>
#include <QFileInfoList>
#include <QStringList>

#include "MediaScanner.h"
#include "MetaDataReader.h"

#include <QDebug>

extern volatile bool forceAbortScan;

MediaScanner::MediaScanner(QObject * parent) : QObject(parent), _recursive(false) {

}

void MediaScanner::scan() {
	if (_path.isEmpty())
		return;

	emit started();

	forceAbortScan = false;

	const QStringList paths = fetchMedia(_path, validFileTypes(), _recursive);

	emit maxItemsChanged(paths.count());

	processMedia(paths);

	emit finished();
}

void MediaScanner::scanPath(const QString & path) {
	_path = path;
	scan();
}

QStringList MediaScanner::validFileTypes() const {
	TagLib::StringList exts = TagLib::FileRef::defaultFileExtensions();
	QStringList types = QString::fromUtf8( exts.toString("%%*.").toCString(true) ).split("%%");

	// Prepends the first items with '*.' also
	types << QString("*.%1").arg( types.takeFirst() );

	return types;
}

QStringList MediaScanner::fetchMedia(const QString & path, const QStringList & fileTypes, bool recursive) const {
	QStringList files;

	if (forceAbortScan)
		return files;

	QDir dir(path);
	QFileInfoList infoList = dir.entryInfoList(fileTypes, QDir::Files, QDir::Name);

	foreach (const QFileInfo info, infoList)
		files << info.absoluteFilePath();

	if (recursive) {
		infoList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

		foreach (const QFileInfo info, infoList)
			files << fetchMedia(info.absoluteFilePath(), fileTypes, true);
	}

	return files;
}

void MediaScanner::processMedia(const QStringList & paths) {
	for (int i = 0; i < paths.count(); i++) {
		if (forceAbortScan)
			return;

		const MetaData metaData = MetaDataReader::getTags(paths[i]);

		if (!metaData.isEmpty())
			emit itemFound(metaData);

		emit itemsDone(i);
	}
}