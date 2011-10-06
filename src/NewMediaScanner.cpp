
#include <taglib/fileref.h>

#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QFuture>

#include "NewMediaScanner.h"

NewMediaScanner::NewMediaScanner(QObject * parent)
: QObject(parent), _recursive(false) {

	_watcher = new QFutureWatcher<MetaData>(this);

	connect(_watcher, SIGNAL(started()), SIGNAL(started()));
	connect(_watcher, SIGNAL(finished()), SIGNAL(finished()));
	connect(_watcher, SIGNAL(canceled()), SIGNAL(finished()));
	connect(_watcher, SIGNAL(progressValueChanged(int)), SIGNAL(itemsDone(int)));
	connect(_watcher, SIGNAL(progressRangeChanged(int, int)), SIGNAL(rangeChanged(int, int)));

	connect(_watcher, SIGNAL(resultReadyAt(int)), SLOT(slotResultReady(int)));
}


void NewMediaScanner::slotResultReady(int i) {
	qDebug() << "Result ready at" << i;
}



void NewMediaScanner::scan(const QString & path) {
	if (!path.isEmpty())
		_path = path;

	if (_path.isEmpty()) {
		qDebug() << "No path set - abort scan";
		return;
	}

	qDebug() << "scan";
}



QStringList NewMediaScanner::validFileTypes() {
	TagLib::StringList exts = TagLib::FileRef::defaultFileExtensions();
	QStringList types = QString::fromUtf8( exts.toString("%%*.").toCString(true) ).split("%%");

	// Prepends the first items with '*.' also
	types << QString("*.%1").arg( types.takeFirst() );

	return types;
}

QStringList NewMediaScanner::fetchMedia() const {
	const QStringList fileTypes = validFileTypes();
	QStringList files;

	QDir dir(_path);
	QFileInfoList infoList = dir.entryInfoList(fileTypes, QDir::Files, QDir::Name);

	while (!infoList.isEmpty()) {
		const QFileInfo info = infoList.takeFirst();
		const QString path = info.absoluteFilePath();

		if (info.isFile())
			files << path;
		else if ( _recursive && info.isDir() ) {
			dir.setPath(path);
			infoList << dir.entryInfoList(fileTypes, QDir::Files, QDir::Name);
		}
	}

	return files;
}


void NewMediaScanner::process(const QString & path) {
	qDebug() << "process path";

}
