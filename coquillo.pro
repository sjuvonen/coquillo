
TEMPLATE = app
TARGET =
DEPENDPATH += . src src/uih
INCLUDEPATH += . src

MOC_DIR = moc
OBJECTS_DIR = obj
UI_HEADERS_DIR = src/uih

QT += network

win32 {
	INCLUDEPATH += resources/win/taglib/include
	LIBS += resources/win/taglib.dll
	CONFIG += windows
	ICON = extra/coquillo.png
}

unix {
	ICON = extra/coquillo.png
	LIBS += -ltag

	executable.path = /usr/bin
	executable.files = coquillo

	icons.path = /usr/share/pixmaps
	icons.files = extra/coquillo.png

	launcher.path = /usr/share/applications
	launcher.files = extra/coquillo.desktop

	INSTALLS += executable icons launcher
}

RESOURCES += coquillo.qrc

FORMS +=   ui/AboutApp.ui \
           ui/CddbSearchDialog.ui \
           ui/MainWindow.ui \
           ui/Processor.ui \
           ui/SettingsPage1.ui \
           ui/SettingsPage2.ui \
           ui/SettingsPage3.ui \
           ui/SettingsPage4.ui \
           ui/TagEditorBasics.ui \
           ui/TagEditorPictures.ui \

HEADERS += src/cddb/Cddb.h src/cddb/CddbPrivate.h
SOURCES += src/cddb/Cddb.cpp src/cddb/CddbPrivate.cpp

HEADERS += src/globals.h \
           src/def_MetaData.h \
           src/AboutDialog.h \
           src/CddbSearchDialog.h \
           src/DirectorySelectorWidget.h \
           src/EditorWidget.h \
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
           src/CddbSearchDialog.cpp \
           src/DirectorySelectorWidget.cpp \
           src/EditorWidget.cpp \
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

