
include(../../libs.pri)

TEMPLATE = lib
TARGET = $$ROOT/lib/coq_editor

FORMS   += $$ROOT/ui/TagEditorBasics.ui \
           $$ROOT/ui/TagEditorPictures.ui \

HEADERS += EditorWidget.h \
           PictureListDelegate.h \
           PicturesTab.h \
           
SOURCES += EditorWidget.cpp \
           PictureListDelegate.cpp \
           PicturesTab.cpp \
