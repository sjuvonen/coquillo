
include(common.pri)

TEMPLATE = subdirs
CONFIG += ordered incremental qt link_prl

SUBDIRS += src/bookmarks src/cddb src/editor src/settings
SUBDIRS += src

win32 {
	CONFIG += windows
	ICON = ../extra/coquillo.png
}

unix {
	ICON = ../extra/coquillo.png

	DATADIR = $$PREFIX/share
	
	icons.path = $$DATADIR/pixmaps
	icons.files = ../extra/coquillo.png

	launcher.path = $$DATADIR/applications
	launcher.files = ../extra/coquillo.desktop

	INSTALLS += icons launcher	
}