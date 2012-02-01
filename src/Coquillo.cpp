
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

// 	ModelDataInspector * inspector = new ModelDataInspector(this);
//	inspector->setModel(_model);

	_window->show();

	QApplication * app = qobject_cast<QApplication *>(QApplication::instance());

	connect(app, SIGNAL(lastWindowClosed()),
		_window, SLOT(saveSettings()));
}

Coquillo::~Coquillo() {
	delete _window;
}

void Coquillo::migrateSettings() {
}

void Coquillo::writeDefaults() {

	QHash<QString, QVariant> defaults;

	// Location to use when starting up the application
	defaults.insert(
		"DefaultLocation",
		QDesktopServices::storageLocation(QDesktopServices::MusicLocation)
	);

	// If this option is enabled, legacy covers will be converted to the new format.
	// If turned off, legacy covers will not be read at all!
	defaults.insert("Compat/ConvertLegacyXiphPictures", true);

	// Pad numbers in filenames to this length when using pad-enabled formatting
	// characters in ProcessorWidget
	defaults.insert("Rules/NumberPadWidth", 2);

	// Don't write useless ID3v1 tags by default
	defaults.insert("Media/MPEG_ID3v1_Enabled", false);
	
	// Default encoding for ID3v2 tags
	defaults.insert("Media/ID3v2_Encoding", "UTF-8");

	QSettings settings;
	
	foreach (QString key, defaults.keys()) {
		if (!settings.contains(key))
			settings.setValue(key, defaults.value(key));
	}

	settings.sync();
}
