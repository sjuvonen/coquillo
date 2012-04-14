
include(../common.pri)

TEMPLATE = app
TARGET = ../coquillo
CONFIG += link_prl

QT += network

DEPENDPATH += . core gui ui
INCLUDEPATH += . core gui

MOC_DIR = ../build
OBJECTS_DIR = ../build
UI_DIR = ../build

LIBS += -L../lib -lcoquillo_core -lcoquillo_gui

unix {
	target.path = $$PREFIX/bin

	INSTALLS += target
}

win32 {
	CONFIG += windows
	INCLUDEPATH += ../resources/windows/taglib/include
	LIBS += ../resources/windows/taglib/taglib.dll
}

FORMS += ui/DirectorySelector.ui ui/MainWindow.ui

HEADERS += Coquillo.h \

SOURCES += Coquillo.cpp \
           main.cpp \

