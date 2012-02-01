
#include <taglib/fileref.h>

#include <QDir>
#include <QFileInfoList>
#include <QStringList>

#include "MediaScanner.h"
#include "MetaDataReader.h"

#include <QDebug>

extern volatile bool abortAction;

MediaScanner::MediaScanner(QObject * parent) : QObject(parent), _recursive(false) {

}

void MediaScanner::scan() {	
	if (_path.isEmpty())
		return;

	emit started();

	const QStringList paths = fetchMedia(_path, validFileTypes(), _recursive);

	emit maximumChanged(paths.count());

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

	if (abortAction)
		return QStringList();

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
		if (abortAction)
			return;

		const MetaData metaData = MetaDataReader::getTags(paths[i]);

		if (metaData.valid())
			emit itemFound(metaData);

		emit progress(i);
	}
}
