
TEMPLATE = app
TARGET = coquillo-bin
DEPENDPATH += . src uih
INCLUDEPATH += . src

MOC_DIR = moc
OBJECTS_DIR = obj
UI_HEADERS_DIR = uih

QT += network

LIBS += -Llib -lcoq_cddb -lcoq_bookmarks

win32 {
	INCLUDEPATH += resources/win/taglib/include
	LIBS += resources/windows/taglib/taglib.dll
	CONFIG += windows
	ICON = extra/coquillo.png
}

unix {
	isEmpty(PREFIX) {
		PREFIX = /usr
	}

	DATADIR = $$PREFIX/share
	BINDIR = $$PREFIX/bin

	ICON = extra/coquillo.png
	LIBS += -ltag

	executable.path = $$BINDIR
	executable.files = coquillo

	icons.path = $$DATADIR/pixmaps
	icons.files = extra/coquillo.png

	launcher.path = $$DATADIR/applications
	launcher.files = extra/coquillo.desktop

	INSTALLS += executable icons launcher
}

RESOURCES += coquillo.qrc

FORMS +=   ui/AboutApp.ui \
           ui/DirectorySelector.ui \
           ui/MainWindow.ui \
           ui/Processor.ui \
           ui/SettingsPage1.ui \
           ui/SettingsPage2.ui \
           ui/SettingsPage3.ui \
           ui/SettingsPage4.ui \
           ui/TagEditorBasics.ui \
           ui/TagEditorPictures.ui \

HEADERS += src/globals.h \
           src/def_MetaData.h \
           src/AboutDialog.h \
           src/DirectorySelectorWidget.h \
           src/EditorWidget.h \
           src/FileSystemProxyModel.h \
           src/ImageCache.h \
           src/MainWindow.h \
           src/MetaDataImage.h \
           src/MetaDataModel.h \
           src/MetaDataReader.h \
           src/MetaDataWriter.h \
           src/ModelDataInspector.h \
           src/MediaScanner.h \
           src/PictureListDelegate.h \
           src/PicturesTab.h \
           src/ProcessorWidget.h \
           src/SettingsDialog.h \
           src/SettingsDialogPage.h \
           src/TableViewMod.h \
           src/SortFilterProxyModelMod.h \

SOURCES += src/globals.cpp \
           src/AboutDialog.cpp \
           src/DirectorySelectorWidget.cpp \
           src/EditorWidget.cpp \
           src/FileSystemProxyModel.cpp \
           src/ImageCache.cpp \
           src/main.cpp \
           src/MainWindow.cpp \
           src/MetaDataImage.cpp \
           src/MetaDataModel.cpp \
           src/MetaDataReader.cpp \
           src/MetaDataWriter.cpp \
           src/ModelDataInspector.cpp \
           src/MediaScanner.cpp \
           src/PictureListDelegate.cpp \
           src/PicturesTab.cpp \
           src/ProcessorWidget.cpp \
           src/SettingsDialog.cpp \
           src/SettingsDialogPage.cpp \
           src/TableViewMod.cpp \
           src/SortFilterProxyModelMod.cpp \
