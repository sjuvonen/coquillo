TEMPLATE = app
TARGET = coquillo

CONFIG += c++11
QT += network widgets concurrent

DEPENDPATH += src
INCLUDEPATH += src

MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build
RCC_DIR = build

unix {
  LIBS += -ltag
  LIBS += -lmusicbrainz5
}

TRANSLATIONS = \
  resources/tr/en.ts \

RESOURCES = resources/translations.qrc

FORMS += \
  ui/basictags.ui \
  ui/filebrowser.ui \
  ui/imagetags.ui \
  ui/mainwindow.ui \
  ui/parserwidget.ui \
  ui/rawdata.ui \
  ui/renamewidget.ui \
  ui/settingsdialog.ui \
  ui/sortpicker.ui \
  ui/tagsearchdialog.ui \

HEADERS += \
  src/headerdatamodel.hpp \
  src/itemcountlabel.hpp \
  src/itemmodifiedproxy.hpp \
  src/mainwindow.hpp \
  src/progresslistener.hpp \
  src/sortpicker.hpp \
  src/stringstoremodel.hpp \
  src/togglewidgetbyaction.hpp \
  src/crawler/crawler.hpp \
  src/crawler/types.hpp \
  src/crawler/tags.hpp \
  src/crawler/tag/generic.hpp \
  src/crawler/tag/id3v2.hpp \
  src/crawler/tag/xiphcomment.hpp \
  src/filebrowser/directorymodel.hpp \
  src/filebrowser/filebrowser.hpp \
  src/processor/basewidget.hpp \
  src/processor/parserwidget.hpp \
  src/processor/patternwidget.hpp \
  src/processor/paths.hpp \
  src/processor/patterns.hpp \
  src/processor/renamewidget.hpp \
  src/settings/settingsdialog.hpp \
  src/tageditor/autonumbers.hpp \
  src/tageditor/basictags.hpp \
  src/tageditor/editorpagebase.hpp \
  src/tageditor/imagemodel.hpp \
  src/tageditor/imagetags.hpp \
  src/tageditor/metadatachangeindicatordelegate.hpp \
  src/tageditor/rawdata.hpp \
  src/tageditor/tageditor.hpp \
  src/tags/image.hpp \
  src/tags/imagecache.hpp \
  src/tags/imagetypes.hpp \
  src/tags/tag.hpp \
  src/tags/tagdataroles.hpp \
  src/tags/tagcontainer.hpp \
  src/tags/tagsmodel.hpp \
  src/tags/tagstore.hpp \
  src/tags/tagwriter.hpp \
  src/tags/tagwriterjob.hpp \
  src/webtags/albumdetailsmodel.hpp \
  src/webtags/tagsearchdialog.hpp \

SOURCES += \
  src/headerdatamodel.cpp \
  src/itemcountlabel.cpp \
  src/itemmodifiedproxy.cpp \
  src/main.cpp \
  src/mainwindow.cpp \
  src/progresslistener.cpp \
  src/sortpicker.cpp \
  src/stringstoremodel.cpp \
  src/togglewidgetbyaction.cpp \
  src/crawler/crawler.cpp \
  src/crawler/tag/generic.cpp \
  src/crawler/tag/id3v2.cpp \
  src/crawler/tag/xiphcomment.cpp \
  src/filebrowser/directorymodel.cpp \
  src/filebrowser/filebrowser.cpp \
  src/processor/basewidget.cpp \
  src/processor/parserwidget.cpp \
  src/processor/patternwidget.cpp \
  src/processor/paths.cpp \
  src/processor/patterns.cpp \
  src/processor/renamewidget.cpp \
  src/settings/settingsdialog.cpp \
  src/tageditor/autonumbers.cpp \
  src/tageditor/basictags.cpp \
  src/tageditor/editorpagebase.cpp \
  src/tageditor/imagemodel.cpp \
  src/tageditor/imagetags.cpp \
  src/tageditor/metadatachangeindicatordelegate.cpp \
  src/tageditor/rawdata.cpp \
  src/tageditor/tageditor.cpp \
  src/tags/image.cpp \
  src/tags/imagecache.cpp \
  src/tags/imagetypes.cpp \
  src/tags/tag.cpp \
  src/tags/tagcontainer.cpp \
  src/tags/tagsmodel.cpp \
  src/tags/tagstore.cpp \
  src/tags/tagwriter.cpp \
  src/webtags/albumdetailsmodel.cpp \
  src/webtags/tagsearchdialog.cpp \

HEADERS += \
  src/searcher/abstractsearcher.hpp \
  src/searcher/musicbrainz.hpp \
  src/searcher/fetchresultjob.hpp \
  src/searcher/searchjob.hpp \

SOURCES += \
  src/searcher/abstractsearcher.cpp \
  src/searcher/musicbrainz.cpp \
  src/searcher/fetchresultjob.cpp \
  src/searcher/searchjob.cpp \

HEADERS += \
  src/varianthashmodel.hpp \
  src/webtags/selectionfiltermodel.hpp \

SOURCES += \
  src/varianthashmodel.cpp \
  src/webtags/selectionfiltermodel.cpp \

HEADERS += \
  src/tags/misc/purgedirsaftercommit.hpp \
  src/tags/misc/renamechangedfiles.hpp \

SOURCES += \
  src/tags/misc/purgedirsaftercommit.cpp \
  src/tags/misc/renamechangedfiles.cpp \
