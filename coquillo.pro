include(common.pri)

TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += src/core src/gui
SUBDIRS += src

unix {
	DATADIR = $$PREFIX/share
	ICON = extra/coquillo.png

	icons.path = $$DATADIR/pixmaps
	icons.files = extra/coquillo.png

	launcher.path = $$DATADIR/applications
	launches.files = extra/coquillo.desktop

	INSTALLS += icons launcher
}

win32 {
	CONFIG += windows
	RC_FILE = coquillo.rc
}
