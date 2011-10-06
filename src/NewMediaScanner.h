#ifndef NEWMEDIASCANNER_H
#define NEWMEDIASCANNER_H

#include <QObject>
#include <QFutureWatcher>

#include "def_MetaData.h"

class NewMediaScanner : public QObject {
	Q_OBJECT

	public:
		NewMediaScanner(QObject * parent=0);

		QString path() const { return _path; }
		void setPath(const QString & p) { _path = p; }

		bool isRecursive() const { return _recursive; }
		void setRecursive(bool state) { _recursive = state; }

	signals:
		void started();
		void finished();

		void progressChanged(int);
		void rangeChanged(int min, int max);

		void found(const MetaData &);

	public slots:
		void scan(const QString & path=QString());

	private slots:
		void slotResultReady(int);

	private:
		static QStringList validFileTypes();
		QStringList fetchMedia() const;

		static process(const QString & path);

		QFutureWatcher<MetaData> * _watcher;

		QString _path;
		bool _recursive;

};

#endif