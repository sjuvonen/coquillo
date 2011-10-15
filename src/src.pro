
include(../common.pri)

TEMPLATE = app
TARGET = ../coquillo

MOC_DIR = ../build
OBJECTS_DIR = ../build
UI_DIR = ../build

LIBS += -L../lib -lcoq_cddb -lcoq_bookmarks -lcoq_editor -lcoq_metadata -lcoq_settings

win32 {
	INCLUDEPATH += ../resources/windows/taglib/include
	LIBS += ../resources/windows/taglib/taglib.dll
}

unix {
	LIBS += -ltag

	# Imports the lib path into the binary
	# QMAKE_RPATHDIR += $$PREFIX/lib/coquillo

	target.path = $$PREFIX/bin

	INSTALLS += target
}

RESOURCES += ../coquillo.qrc

FORMS +=   ../ui/AboutApp.ui \
           ../ui/DirectorySelector.ui \
           ../ui/MainWindow.ui \
           ../ui/Processor.ui \
           ../ui/TagEditorBasics.ui \
           ../ui/TagEditorPictures.ui \

HEADERS += globals.h \
           AboutDialog.h \
           DirectorySelectorWidget.h \
           FileSystemProxyModel.h \
           MainWindow.h \
           ModelDataInspector.h \
           ProcessorWidget.h \
           SortFilterProxyModelMod.h \

SOURCES += main.cpp \
           globals.cpp \
           AboutDialog.cpp \
           DirectorySelectorWidget.cpp \
           FileSystemProxyModel.cpp \
           MainWindow.cpp \
           ModelDataInspector.cpp \
           ProcessorWidget.cpp \
           SortFilterProxyModelMod.cpp \
           
