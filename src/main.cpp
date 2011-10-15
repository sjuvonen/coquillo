#include <iostream>

#include <QApplication>
#include <QMainWindow>
#include <QSettings>
#include <QTextCodec>

#include <QDebug>

#include "globals.h"

#include "MainWindow.h"

#include <QByteArray>
#include <QDataStream>

/**
 * NOTE-TO-SELF: these defines are updated by the build script!
 **/
#define APP_VERSION "git"
#define RELEASE_DATE "-"

const QString Coquillo::appVersion = APP_VERSION;
const QString Coquillo::releaseDate = RELEASE_DATE;

QMap<QString, QString> extractArgs(int & argc, char ** args);
void migrateSettings();
void printHelp();
int versionStringToInt(const QString &);

int main(int argc, char ** args) {
	QApplication app(argc, args);


	Coquillo::fieldNames.insert(0, "Title");
	Coquillo::fieldNames.insert(1, "Artist");
	Coquillo::fieldNames.insert(2, "Album");
	Coquillo::fieldNames.insert(3, "Year");
	Coquillo::fieldNames.insert(4, "Disc");
	Coquillo::fieldNames.insert(5, "Number");
	Coquillo::fieldNames.insert(6, "MaxNumber");
	Coquillo::fieldNames.insert(7, "Genre");
	Coquillo::fieldNames.insert(8, "Comment");
	Coquillo::fieldNames.insert(9, "OriginalArtist");
	Coquillo::fieldNames.insert(10, "Composer");
	Coquillo::fieldNames.insert(11, "Url");
	Coquillo::fieldNames.insert(12, "Encoder");
	Coquillo::fieldNames.insert(13, "Path");
	Coquillo::fieldNames.insert(14, "Pictures");
	Coquillo::fieldNames.insert(15, "Length");

	QCoreApplication::setApplicationName("Coquillo");
	QCoreApplication::setOrganizationName("Smusoft");
	QCoreApplication::setOrganizationDomain("univerge.no-ip.org");
	QCoreApplication::setApplicationVersion(Coquillo::appVersion);

	migrateSettings();

	// Global settings
	QSettings settings;

	bool convext = settings.value("Processing/ConvertExtensions").toBool();
	bool lower = settings.value("Processing/LowerCaseExtensions").toBool();

	Coquillo::fileExtensionCase = convext ? (lower ? 1 : 2) : 0;
	Coquillo::numberPadWidth = settings.value("Processing/PadWidth").toInt();
	Coquillo::intelligentCapitalization = settings.value("Processing/IntelligentCapitalization").toBool();
	Coquillo::encoding = settings.value("Application/Encoding").toString() == "UTF-8" ? Coquillo::UTF_8 : Coquillo::ISO_8859_15;
	Coquillo::safeFileNames = settings.value("Processing/SafeFileNames").toBool();
	Coquillo::trimWhiteSpace = settings.value("Processing/TrimWhiteSpace").toBool();
	Coquillo::removeDiscFromAlbumName = settings.value("Processing/RemoveDiscFromAlbumName").toBool();

	Coquillo::Flac::id3v2 = settings.value("FileTypes/FLAC/WriteID3v2").toBool();
	Coquillo::Mpeg::id3v1 = settings.value("FileTypes/MPEG/WriteID3v1").toBool();
	Coquillo::OggVorbis::stripLegacyCovers = settings.value("FileTypes/OggVorbis/StripLegacyCovers").toBool();

	const QString enc = settings.value("General/Encoding", "UTF-8").toString();
	Coquillo::encoding = ( enc == "UTF-8" ? Coquillo::UTF_8 : Coquillo::ISO_8859_15 );

	Coquillo::dimSubdirectoriesBackground = settings.value("MainWindow/DimSubDirectoriesRows").toBool();

	QTextCodec::setCodecForCStrings( QTextCodec::codecForName(enc.toLocal8Bit()) );

	QMap<QString, QString> arguments = extractArgs(argc, args);

	if (arguments.contains("--help")) {
		printHelp();
		return 0;
	}

	MainWindow * mw = new MainWindow;
	mw->show();

	if (arguments.contains("--dir"))
		mw->openDirectory(arguments["--dir"]);

	return app.exec();
}

QMap<QString, QString> extractArgs(int & argc, char ** args) {
	QMap<QString, QString> map;

	// Skip filename
	args++;

	for (int i = 1; i < argc; i++) {
		if (strcmp("--help", *args) == 0)
			map.insert("--help", QString());
		else
			map.insert("--dir", QString(*args));

		args++;
	}

	return map;

}

#include <QDebug>

void migrateSettings() {
	// Resets some old settings keys that would otherwise mess up the application.
	// Also just cleans up old clutter from the settings.

	QSettings settings;

	int v = versionStringToInt( settings.value("Application/SettingsVersion").toString() );

	switch (v) {
		case 103:
			settings.remove("FileTypes/MPEG/WriteID3V2");
			settings.remove("MainWindow/TableHeader");

			settings.setValue("Application/Encoding", settings.value("General/Encoding"));
			settings.remove("General");

			break;

		case 105:
			// Due to a bug in version 1.5 this group was still present.
			settings.remove("General");
	}

	settings.setValue("Application/SettingsVersion", Coquillo::appVersion);

	settings.sync();
}

// Major version is multiplied with 100 and minor version is added to it.
// Therefore 1.3 would be 103, 1.5 would be 105 and 1.20 would be 120.
int versionStringToInt(const QString & version) {
	return version.section('.', 0, 0).toInt() * 100 + version.section('.', -1).toInt();
}

void printHelp() {
	std::cout << "Usage: " << "coquillo [OPTION...] [PATH]" << std::endl;
	std::cout << std::endl;
	std::cout << "Valid options:" << std::endl;
	std::cout << "\t--help\tDisplay this help" << std::endl;
}

