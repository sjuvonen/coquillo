TEMPLATE = app
TARGET = coquillo
QT += widgets

DEPENDPATH += src src/filebrowser src/metadata
INCLUDEPATH += src

MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build

unix {
    LIBS += -ltag
}

FORMS += \
    ui/filebrowser.ui \
    ui/basictags.ui \
    ui/mainwindow.ui \
    ui/parserwidget.ui \
    ui/renamewidget.ui \

HEADERS += \
    src/mainwindow.h \
    src/stringstoremodel.h \
    src/filebrowser/directorymodel.h \
    src/filebrowser/filebrowser.h \
    src/metadata/filereader.h \
    src/metadata/filetypes.h \
    src/metadata/mediacrawler.h \
    src/metadata/metadata.h \
    src/metadata/metadatamodel.h \
    src/metadata/tagreader.h \
    src/processor/basewidget.h \
    src/processor/parserwidget.h \
    src/processor/patternwidget.h \
    src/processor/patterns.h \
    src/processor/renamewidget.h \
    src/tageditor/basictags.h \
    src/tageditor/metadatachangeindicatordelegate.h \

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/stringstoremodel.cpp \
    src/filebrowser/directorymodel.cpp \
    src/filebrowser/filebrowser.cpp \
    src/metadata/filereader.cpp \
    src/metadata/filetypes.cpp \
    src/metadata/mediacrawler.cpp \
    src/metadata/metadata.cpp \
    src/metadata/metadatamodel.cpp \
    src/metadata/tagreader.cpp \
    src/processor/basewidget.cpp \
    src/processor/parserwidget.cpp \
    src/processor/patternwidget.cpp \
    src/processor/patterns.cpp \
    src/processor/renamewidget.cpp \
    src/tageditor/basictags.cpp \
    src/tageditor/metadatachangeindicatordelegate.cpp \
