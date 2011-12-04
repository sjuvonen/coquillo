
include(../../libs.pri)

TEMPLATE = lib
TARGET = $$ROOT/lib/coq_core

# Input
HEADERS += ImageCache.h \
           MediaScanner.h \
           MetaDataImage.h \
           MetaDataModel.h \
           MetaDataReader.h \
           MetaDataWriter.h \
           ModelDataInspector.h \
           ../globals.h \

SOURCES += ImageCache.cpp \
           MediaScanner.cpp \
           MetaDataImage.cpp \
           MetaDataModel.cpp \
           MetaDataReader.cpp \
           MetaDataWriter.cpp \
           ModelDataInspector.cpp \
           ../globals.cpp \
