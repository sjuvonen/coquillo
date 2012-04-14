
#define APPLICATION_VERSION "git"
#define RELEASE_DATE "-"

#include <iostream>

#include <QApplication>
#include <QDebug>
#include <QStringList>

#include "Coquillo.h"

void printHelp();

const char * releaseDate = RELEASE_DATE;
const char * applicationVersion = APPLICATION_VERSION;

int main(int argc, char ** argv) {
	QApplication app(argc, argv);

	QCoreApplication::setApplicationName("Coquillo");
	QCoreApplication::setOrganizationName("Smusoft");
	QCoreApplication::setOrganizationDomain("univerge.no-ip.org");
	QCoreApplication::setApplicationVersion(::applicationVersion);

	const QStringList args = QApplication::arguments();

	if (args.contains("--help")) {
		// Print help and quit.
		printHelp();
		return 0;
	}

	Coquillo coquillo;

	return app.exec();
}

void printHelp() {
	std::cout << "Usage: coquillo [options] [dir]" << std::endl << std::endl;
	std::cout << "Coquillo " << APPLICATION_VERSION << std::endl << std::endl;
	std::cout << "Audio file metadata editor" << std::endl;
}
