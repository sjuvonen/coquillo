
include(../common.pri)

TEMPLATE = app
TARGET = ../coquillo

MOC_DIC = ../build
OBJECTS_DIR = ../build
UI_DIR = ../build

LIBS += -L../lib -lcoq_cddb -lcoq_bookmarks -lcoq_editor -lcoq_settings

win32 {
	INCLUDEPATH += ../resources/win/taglib/include
	LIBS += ../resources/windows/taglib/taglib.dll
}

unix {
	LIBS += -ltag

	# Set the LD_LIBRARY_PATH
	QMAKE_RPATHDIR += $$PREFIX/lib/coquillo

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

HEADERS += def_MetaData.h \
           globals.h \
           AboutDialog.h \
           DirectorySelectorWidget.h \
           FileSystemProxyModel.h \
           ImageCache.h \
           MainWindow.h \
           MediaScanner.h \
           MetaDataImage.h \
           MetaDataModel.h \
           MetaDataReader.h \
           MetaDataWriter.h \
           ModelDataInspector.h \
           ProcessorWidget.h \
           SortFilterProxyModelMod.h \

SOURCES += main.cpp \
           globals.cpp \
           AboutDialog.cpp \
           DirectorySelectorWidget.cpp \
           FileSystemProxyModel.cpp \
           ImageCache.cpp \
           MainWindow.cpp \
           MediaScanner.cpp \
           MetaDataImage.cpp \
           MetaDataModel.cpp \
           MetaDataReader.cpp \
           MetaDataWriter.cpp \
           ModelDataInspector.cpp \
           ProcessorWidget.cpp \
           SortFilterProxyModelMod.cpp \
           
