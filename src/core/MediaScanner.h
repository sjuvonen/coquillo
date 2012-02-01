#ifndef MEDIASCANNER_H
#define MEDIASCANNER_H

#include <QObject>

#include "MetaData.h"

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
		void progress(int);
		void itemFound(const MetaData &);
		void maximumChanged(int);
		void started();

	public slots:
		void scan();
		void scanPath(const QString & path);

	private:
		QStringList validFileTypes() const;
		QStringList fetchMedia(const QString & path, const QStringList & fileTypes, bool recursive) const;

		void processMedia(const QStringList & paths);

		static void process(const QString & filePath);

		bool _recursive;
		QString _path;
};

#endif
