
include(../common.pri)

TEMPLATE = app
TARGET = ../coquillo

DEPENDPATH += . core gui ui
INCLUDEPATH += . core gui

MOC_DIR = ../build
OBJECTS_DIR = ../build
UI_DIR = ../build

unix {

	# Imports the lib path into the binary
	# QMAKE_RPATHDIR += $$PREFIX/lib/coquillo

	target.path = $$PREFIX/bin

	INSTALLS += target
}

win32 {
	INCLUDEPATH += ../resources/windows/taglib/include
	LIBS += ../resources/windows/taglib/taglib.dll
}

LIBS += -L../lib -lcoquillo_core -lcoquillo_gui

FORMS += ui/DirectorySelector.ui ui/MainWindow.ui

HEADERS += Coquillo.h \

SOURCES += Coquillo.cpp \
           main.cpp \

