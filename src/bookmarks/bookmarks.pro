
include(../../libs.pri)

TEMPLATE = lib
TARGET = $$ROOT/lib/coq_bookmarks

# Input
FORMS   += $$ROOT/ui/BookmarkDialog.ui
HEADERS += BookmarkDialog.h BookmarkModel.h
SOURCES += BookmarkDialog.cpp BookmarkModel.cpp
