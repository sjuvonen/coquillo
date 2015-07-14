TEMPLATE = app
TARGET = coquillo

CONFIG += c++11
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
    ui/audioproperties.ui \
    ui/basictags.ui \
    ui/filebrowser.ui \
    ui/imagetags.ui \
    ui/mainwindow.ui \
    ui/parserwidget.ui \
    ui/rawdata.ui \
    ui/renamewidget.ui \
    ui/settingsdialog.ui \
    ui/tagsearchdialog.ui \

HEADERS += \
    src/actionsignalmapper.hpp \
    src/itemcountlabel.hpp \
    src/mainwindow.hpp \
    src/stringstoremodel.hpp \
    src/togglewidgetbyaction.hpp \
    src/filebrowser/directorymodel.hpp \
    src/filebrowser/filebrowser.hpp \
    src/filters/abstractfilter.hpp \
    src/filters/parsediscnumber.hpp \
    src/filters/safefilenames.hpp \
    src/filters/scaleimages.hpp \
    src/filters/trimwhitespace.hpp \
    src/metadata/filereader.hpp \
    src/metadata/filewriter.hpp \
    src/metadata/image.hpp \
    src/metadata/imagecache.hpp \
    src/metadata/imagetypes.hpp \
    src/metadata/itemdataroles.hpp \
    src/metadata/mapper.hpp \
    src/metadata/mediacrawler.hpp \
    src/metadata/metadata.hpp \
    src/metadata/metadatamodel.hpp \
    src/metadata/metadatastore.hpp \
    src/metadata/tags/default.hpp \
    src/metadata/tags/id3v2.hpp \
    src/metadata/tags/xiphcomment.hpp \
    src/processor/basewidget.hpp \
    src/processor/parserwidget.hpp \
    src/processor/patternwidget.hpp \
    src/processor/patterns.hpp \
    src/processor/renamewidget.hpp \
    src/searcher/abstractsearcher.hpp \
    src/searcher/musicbrainz.hpp \
    src/settings/settingsdialog.hpp \
    src/tageditor/audioproperties.hpp \
    src/tageditor/autonumbers.hpp \
    src/tageditor/basictags.hpp \
    src/tageditor/imagemodel.hpp \
    src/tageditor/imagetags.hpp \
    src/tageditor/metadatachangeindicatordelegate.hpp \
    src/tageditor/rawdata.hpp \
    src/tageditor/tageditor.hpp \
    src/webtags/filterproxymodel.hpp \
    src/webtags/tagsearchdialog.hpp \
    src/webtags/trackselectionmodel.hpp \
    # Experimental
    src/quick/metadataproxymodel.hpp \

SOURCES += \
    src/main.cpp \
    src/actionsignalmapper.cpp \
    src/itemcountlabel.cpp \
    src/mainwindow.cpp \
    src/stringstoremodel.cpp \
    src/togglewidgetbyaction.cpp \
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
    src/metadata/metadatastore.cpp \
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
    src/tageditor/audioproperties.cpp \
    src/tageditor/autonumbers.cpp \
    src/tageditor/basictags.cpp \
    src/tageditor/imagemodel.cpp \
    src/tageditor/imagetags.cpp \
    src/tageditor/metadatachangeindicatordelegate.cpp \
    src/tageditor/rawdata.cpp \
    src/tageditor/tageditor.cpp \
    src/webtags/filterproxymodel.cpp \
    src/webtags/tagsearchdialog.cpp \
    src/webtags/trackselectionmodel.cpp \
    # Experimental
    src/quick/metadataproxymodel.cpp \
