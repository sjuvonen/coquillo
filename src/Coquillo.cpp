
#include <QApplication>
#include <QDesktopServices>
#include <QSettings>

#include "Coquillo.h"

#include <core/MetaDataModel2.h>
#include <core/ModelDataInspector.h>
#include <gui/MainWindow.h>

Coquillo::Coquillo(QObject * parent)
: QObject(parent) {

	migrateSettings();
	writeDefaults();

	MetaDataModel2 * m = new MetaDataModel2(this);

	_window = new MainWindow(m);

	ModelDataInspector * inspector = new ModelDataInspector(this);
	inspector->setModel(m);

	_window->show();

	QApplication * app = qobject_cast<QApplication *>(QApplication::instance());

	connect(app, SIGNAL(lastWindowClosed()),
		_window, SLOT(saveSettings()));
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

	defaults.insert("CapitalizeNumerals", true);
	defaults.insert("Widgets/MainWindowSize", QSize(940, 600));

	QSettings settings;
	
	foreach (QString key, defaults.keys()) {
		if (!settings.contains(key))
			settings.setValue(key, defaults.value(key));
	}
	
	settings.sync();
}
