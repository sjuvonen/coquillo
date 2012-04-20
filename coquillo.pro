include(common.pri)

TEMPLATE = app

QT += network

MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build

INCLUDEPATH += src/core src/gui
INCLUDEPATH += src/gui/bookmarks/ src/gui/cddb/
INCLUDEPATH += src/

RESOURCES += coquillo.qrc

unix {
	DATADIR = $$PREFIX/share
	ICON = extra/coquillo.png

	icons.path = $$DATADIR/pixmaps
	icons.files = extra/coquillo.png

	launcher.path = $$DATADIR/applications
	launcher.files = extra/coquillo.desktop

	target.path = $$PREFIX/bin

	INSTALLS += target
	INSTALLS += icons launcher

	LIBS += -ltag
}

win32 {
	INCLUDEPATH += resources/windows/taglib/include
	LIBS += resources/windows/taglib/taglib.dll
	
	CONFIG += windows
	RC_FILE = coquillo.rc
}

FORMS +=   src/ui/MainWindow.ui \
           src/ui/AboutApp.ui \
           src/ui/BookmarkDialog.ui \
           src/ui/CddbSearchDialog.ui \
           src/ui/DirectorySelector.ui \
           src/ui/EditorWidget.ui \
           src/ui/ProcessorWidget.ui \
					 src/ui/SettingsDialog.ui \

HEADERS += src/Coquillo.h \

SOURCES += src/Coquillo.cpp \
           src/main.cpp \

HEADERS += src/core/globals.h \
           src/core/MediaScanner.h \
           src/core/MetaData.h \
           src/core/MetaDataImage.h \
           src/core/MetaDataModel2.h \
           src/core/MetaDataReader.h \
           src/core/MetaDataWriter.h \
					 src/core/ModelDataInspector.h \

SOURCES += src/core/globals.cpp \
           src/core/MediaScanner.cpp \
           src/core/MetaData.cpp \
           src/core/MetaDataImage.cpp \
           src/core/MetaDataModel2.cpp \
           src/core/MetaDataReader.cpp \
           src/core/MetaDataWriter.cpp \
					 src/core/ModelDataInspector.cpp \

HEADERS += src/gui/DirectorySelectorWidget.h \
           src/gui/DataWidget.h \
           src/gui/EditorWidget.h \
           src/gui/FileSystemProxyModel.h \
           src/gui/MainWindow.h \
           src/gui/ProcessorWidget.h \

SOURCES += src/gui/DirectorySelectorWidget.cpp \
           src/gui/DataWidget.cpp \
           src/gui/EditorWidget.cpp \
           src/gui/FileSystemProxyModel.cpp \
           src/gui/MainWindow.cpp \
           src/gui/ProcessorWidget.cpp \

HEADERS += src/gui/bookmarks/BookmarkDialog.h \
           src/gui/bookmarks/BookmarkModel.h \

SOURCES += src/gui/bookmarks/BookmarkDialog.cpp \
           src/gui/bookmarks/BookmarkModel.cpp \

HEADERS += src/gui/cddb/Cddb.h \
           src/gui/cddb/CddbPrivate.h \
           src/gui/cddb/CddbSearchDialog.h \

SOURCES += src/gui/cddb/Cddb.cpp \
           src/gui/cddb/CddbPrivate.cpp \
           src/gui/cddb/CddbSearchDialog.cpp \

HEADERS += src/gui/AboutDialog.h \
           src/gui/SettingsDialog.h \

SOURCES += src/gui/AboutDialog.cpp \
           src/gui/SettingsDialog.cpp \
