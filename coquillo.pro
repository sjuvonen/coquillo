TEMPLATE = app
TARGET = coquillo

QT += network widgets

DEPENDPATH += src src/filebrowser src/metadata
INCLUDEPATH += src

MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build

unix {
    LIBS += -ltag
    LIBS += -lmusicbrainz5
}

FORMS += \
    ui/filebrowser.ui \
    ui/basictags.ui \
    ui/imagetags.ui \
    ui/mainwindow.ui \
    ui/parserwidget.ui \
    ui/rawdata.ui \
    ui/renamewidget.ui \
    ui/settingsdialog.ui \
    ui/tagsearchdialog.ui \

HEADERS += \
    src/actionsignalmapper.h \
    src/mainwindow.h \
    src/stringstoremodel.h \
    src/filebrowser/directorymodel.h \
    src/filebrowser/filebrowser.h \
    src/filters/abstractfilter.h \
    src/filters/parsediscnumber.h \
    src/filters/safefilenames.h \
    src/filters/scaleimages.h \
    src/filters/trimwhitespace.h \
    src/metadata/filereader.h \
    src/metadata/filewriter.h \
    src/metadata/image.h \
    src/metadata/imagecache.h \
    src/metadata/imagetypes.h \
    src/metadata/mapper.h \
    src/metadata/mediacrawler.h \
    src/metadata/metadata.h \
    src/metadata/metadatamodel.h \
    src/metadata/tags/default.h \
    src/metadata/tags/id3v2.h \
    src/metadata/tags/xiphcomment.h \
    src/processor/basewidget.h \
    src/processor/parserwidget.h \
    src/processor/patternwidget.h \
    src/processor/patterns.h \
    src/processor/renamewidget.h \
    src/searcher/abstractsearcher.h \
    src/searcher/musicbrainz.h \
    src/settings/settingsdialog.h \
    src/tageditor/basictags.h \
    src/tageditor/imagemodel.h \
    src/tageditor/imagetags.h \
    src/tageditor/metadatachangeindicatordelegate.h \
    src/tageditor/rawdata.h \
    src/tageditor/tageditor.h \
    src/webtags/filterproxymodel.h \
    src/webtags/tagsearchdialog.h \
    # Experimental
    src/quick/metadataproxymodel.h \

SOURCES += \
    src/main.cpp \
    src/actionsignalmapper.cpp \
    src/mainwindow.cpp \
    src/stringstoremodel.cpp \
    src/filters/parsediscnumber.cpp \
    src/filters/safefilenames.cpp \
    src/filters/scaleimages.cpp \
    src/filters/trimwhitespace.cpp \
    src/filebrowser/directorymodel.cpp \
    src/filebrowser/filebrowser.cpp \
    src/metadata/filereader.cpp \
    src/metadata/filewriter.cpp \
    src/metadata/image.cpp \
    src/metadata/imagecache.cpp \
    src/metadata/imagetypes.cpp \
    src/metadata/mapper.cpp \
    src/metadata/mediacrawler.cpp \
    src/metadata/metadata.cpp \
    src/metadata/metadatamodel.cpp \
    src/metadata/tags/default.cpp \
    src/metadata/tags/id3v2.cpp \
    src/metadata/tags/xiphcomment.cpp \
    src/processor/basewidget.cpp \
    src/processor/parserwidget.cpp \
    src/processor/patternwidget.cpp \
    src/processor/patterns.cpp \
    src/processor/renamewidget.cpp \
    src/searcher/abstractsearcher.cpp \
    src/searcher/musicbrainz.cpp \
    src/settings/settingsdialog.cpp \
    src/tageditor/basictags.cpp \
    src/tageditor/imagemodel.cpp \
    src/tageditor/imagetags.cpp \
    src/tageditor/metadatachangeindicatordelegate.cpp \
    src/tageditor/rawdata.cpp \
    src/tageditor/tageditor.cpp \
    src/webtags/filterproxymodel.cpp \
    src/webtags/tagsearchdialog.cpp \
    # Experimental
    src/quick/metadataproxymodel.cpp \
