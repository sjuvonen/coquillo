include(../../libs.pri)

TEMPLATE = lib
TARGET = $$ROOT/lib/coquillo_core
DEPENDPATH += .
INCLUDEPATH += .

LIBS += -ltag

HEADERS += MetaDataModel2.h
SOURCES += MetaDataModel2.cpp

# Input
HEADERS += globals.h \
           MediaScanner.h \
           MetaData.h \
           MetaDataImage.h \
           MetaDataModel.h \
           MetaDataReader.h \
           MetaDataWriter.h \
					 ModelDataInspector.h \

SOURCES += globals.cpp \
           MediaScanner.cpp \
           MetaData.cpp \
           MetaDataImage.cpp \
           MetaDataModel.cpp \
           MetaDataReader.cpp \
           MetaDataWriter.cpp \
					 ModelDataInspector.cpp \

