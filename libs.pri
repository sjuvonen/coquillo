
include(common.pri)

ROOT = ../../

MOC_DIR = $$ROOT/build
OBJECTS_DIR = $$ROOT/build
UI_DIR = $$ROOT/build

# Remember to update the spec files after changing this!
VERSION = 1.0

unix {
	target.path = $$PREFIX/lib/
	INSTALLS += target
}

win32 {
	CONFIG += dll
}