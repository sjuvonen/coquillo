#ifndef COQUILLO_H
#define COQUILLO_H

#include <QObject>

class QNetworkAccessManager;

class MainWindow;

class Coquillo : public QObject {
	Q_OBJECT

	public:
		Coquillo(QObject * parent=0);
		~Coquillo();

	private:
		void migrateSettings();
		void writeDefaults();

		QNetworkAccessManager * _networkManager;
		
		MainWindow * _window;
};

#endif