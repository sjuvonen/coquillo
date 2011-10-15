
include(common.pri)

INCLUDEPATH += ..

ROOT = ../../

MOC_DIR = $$ROOT/build
OBJECTS_DIR = $$ROOT/build
UI_DIR = $$ROOT/build

VERSION = 1.0

target.path = $$PREFIX/lib/coquillo
INSTALLS += target

win32 {
	INCLUDEPATH += $$ROOT/resources/windows/taglib/include
	LIBS += $$ROOT/resources/windows/taglib/taglib.dll

	CONFIG += windows dll
}