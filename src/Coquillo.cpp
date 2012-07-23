
#include <QApplication>
#include <QDebug>
#include <QDesktopServices>
#include <QFileInfo>
#include <QNetworkAccessManager>
#include <QSettings>

#include "Coquillo.h"

#include <core/MetaDataModel2.h>
#include <core/ModelDataInspector.h>
#include <gui/MainWindow.h>

Coquillo::Coquillo(QObject * parent)
: QObject(parent) {

	_networkManager = new QNetworkAccessManager(this);

	migrateSettings();
	writeDefaults();

	MetaDataModel2 * m = new MetaDataModel2(this);
	m->setNetworkManager(_networkManager);

	_window = new MainWindow(m);

	ModelDataInspector * inspector = new ModelDataInspector(this);
	inspector->setModel(m);

	QString location = QSettings().value("DefaultLocation").toString();

	if (QApplication::arguments().count() > 1) {
		const QFileInfo info(QApplication::arguments().last());

		if (info.exists() && info.isDir()) {
			location = info.absoluteFilePath();
		} else {
			qWarning() << "Error: Path" << info.absoluteFilePath() << "does not exist!";
		}
	}

	m->setDirectory(location);

	_window->show();
	_window->togglePlayerWidget(false);
}

Coquillo::~Coquillo() {
	delete _window;
}

void Coquillo::migrateSettings() {
	QSettings settings;

	const int version = settings.value("Settings/Version").toInt();

	// Reset settings for Coquillo versions < 1.10
	// We don't bother trying to migrate older versions due to exhaustive
	// amount of breakage
	if (version < 10) {
		foreach (const QString key, settings.allKeys())
			settings.remove(key);
	}

	// Version 10 since Coquillo 1.10
	settings.setValue("Settings/Version", 10);
}

void Coquillo::writeDefaults() {

	QHash<QString, QVariant> defaults;

	// Location to use when starting up the application
	defaults.insert(
		"DefaultLocation",
		QDesktopServices::storageLocation(QDesktopServices::MusicLocation)
	);

	defaults.insert("CapitalizeNever", QString("%1%2")
		.arg("a, an, the, and, but, or, nor, at, by, for, from, in, into, of, ")
		.arg("off, on, onto, out, over, to, up, with, as"));

	defaults.insert("Widgets/MainWindowSize", QSize(940, 600));

	// Whether or not make Roman numerals always capitalized
	defaults.insert("CapitalizeRomanNumerals", true);

	// Substring length to use with substr patterns (%A, %B, %T, %G) in processor
	defaults.insert("SubstringPatternLength", 1);

	QSettings settings;

	foreach (QString key, defaults.keys()) {
		if (!settings.contains(key))
			settings.setValue(key, defaults.value(key));
	}

	settings.sync();
}
