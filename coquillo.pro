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
  ui/mainwindow.ui \

HEADERS += \
  src/mainwindow.hpp \
  src/crawler/crawler.hpp \
  src/crawler/tag/generic.hpp \
  src/crawler/tag/id3v2tag.hpp \
  src/crawler/tag/xiphcomment.hpp \
  src/tags/tag.hpp \
  src/tags/tagcontainer.hpp \
  src/tags/tagsmodel.hpp \
  src/tags/tagstore.hpp \

SOURCES += \
  src/main.cpp \
  src/mainwindow.cpp \
  src/crawler/crawler.cpp \
  src/crawler/tag/generic.cpp \
  src/crawler/tag/id3v2tag.cpp \
  src/crawler/tag/xiphcomment.cpp \
  src/tags/tag.cpp \
  src/tags/tagcontainer.cpp \
  src/tags/tagsmodel.cpp \
  src/tags/tagstore.cpp \
