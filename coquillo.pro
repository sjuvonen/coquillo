TEMPLATE = app
TARGET = coquillo

CONFIG += c++11
QT += network widgets concurrent

DEPENDPATH += src
INCLUDEPATH += src

MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build

unix {
  LIBS += -ltag
  LIBS += -lmusicbrainz5
}

FORMS += \
  ui/basictags.ui \
  ui/filebrowser.ui \
  ui/mainwindow.ui \
  ui/rawdata.ui \
  ui/settingsdialog.ui \

HEADERS += \
  src/itemcountlabel.hpp \
  src/mainwindow.hpp \
  src/stringstoremodel.hpp \
  src/crawler/crawler.hpp \
  src/crawler/tag/generic.hpp \
  src/crawler/tag/id3v2tag.hpp \
  src/crawler/tag/xiphcomment.hpp \
  src/filebrowser/directorymodel.hpp \
  src/filebrowser/filebrowser.hpp \
  src/settings/settingsdialog.hpp \
  src/tageditor/basictags.hpp \
  src/tageditor/metadatachangeindicatordelegate.hpp \
  src/tageditor/rawdata.hpp \
  src/tageditor/tageditor.hpp \
  src/tags/tag.hpp \
  src/tags/tagdataroles.hpp \
  src/tags/tagcontainer.hpp \
  src/tags/tagsmodel.hpp \
  src/tags/tagstore.hpp \

SOURCES += \
  src/itemcountlabel.cpp \
  src/main.cpp \
  src/mainwindow.cpp \
  src/stringstoremodel.cpp \
  src/crawler/crawler.cpp \
  src/crawler/tag/generic.cpp \
  src/crawler/tag/id3v2tag.cpp \
  src/crawler/tag/xiphcomment.cpp \
  src/filebrowser/directorymodel.cpp \
  src/filebrowser/filebrowser.cpp \
  src/settings/settingsdialog.cpp \
  src/tageditor/basictags.cpp \
  src/tageditor/metadatachangeindicatordelegate.cpp \
  src/tageditor/rawdata.cpp \
  src/tageditor/tageditor.cpp \
  src/tags/tag.cpp \
  src/tags/tagcontainer.cpp \
  src/tags/tagsmodel.cpp \
  src/tags/tagstore.cpp \
