
#define APPLICATION_VERSION "git"
#define RELEASE_DATE "-"

#include <QApplication>
#include "Coquillo.h"

const char * releaseDate = "n/a";
const char * applicationVersion = "git";

int main(int argc, char ** argv) {
	QApplication app(argc, argv);

	QCoreApplication::setApplicationName("Coquillo2");
	QCoreApplication::setOrganizationName("Smusoft");
	QCoreApplication::setOrganizationDomain("univerge.no-ip.org");
	QCoreApplication::setApplicationVersion(::applicationVersion);

	Coquillo coquillo;

	return app.exec();
}