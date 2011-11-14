
include(../../libs.pri)

TEMPLATE = lib
TARGET = $$ROOT/lib/coq_cddb

QT += network

FORMS   += $$ROOT/ui/CddbSearchDialog.ui
HEADERS += Cddb.h CddbPrivate.h CddbSearchDialog.h
SOURCES += Cddb.cpp CddbPrivate.cpp CddbSearchDialog.cpp

