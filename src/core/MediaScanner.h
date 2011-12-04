#ifndef MEDIASCANNER_H
#define MEDIASCANNER_H

#include <QObject>

#include "../globals.h"

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

		void rangeChanged(int min, int max);

	public slots:
		void scan();
		void scanPath(const QString & path);

	/*
	 private slots:
		void slotResultReady(int i);
		void slotResultsReady(int i, int j);
	*/

	private:
		QStringList validFileTypes() const;
		QStringList fetchMedia(const QString & path, const QStringList & fileTypes, bool recursive) const;

		void processMedia(const QStringList & paths);

		static void process(const QString & filePath);

		bool _recursive;
		QString _path;
};

#endif
