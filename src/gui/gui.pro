include(../../libs.pri)

TEMPLATE = lib
TARGET = $$ROOT/lib/coquillo_gui
DEPENDPATH += .
INCLUDEPATH += .

QT += network

LIBS += -L$$ROOT/lib -lcoquillo_core
INCLUDEPATH += ../ bookmarks/ cddb/

RESOURCES += $$ROOT/coquillo.qrc

FORMS +=   ../ui/MainWindow.ui \
           ../ui/AboutApp.ui \
           ../ui/BookmarkDialog.ui \
           ../ui/CddbSearchDialog.ui \
           ../ui/DirectorySelector.ui \
           ../ui/EditorWidget.ui \
           ../ui/ProcessorWidget.ui \

HEADERS += DirectorySelectorWidget.h \
           DataWidget.h \
           EditorWidget.h \
           FileSystemProxyModel.h \
           MainWindow.h \
           ProcessorWidget.h \

SOURCES += DirectorySelectorWidget.cpp \
           DataWidget.cpp \
           EditorWidget.cpp \
           FileSystemProxyModel.cpp \
           MainWindow.cpp \
           ProcessorWidget.cpp \

HEADERS += bookmarks/BookmarkDialog.h \
           bookmarks/BookmarkModel.h \

SOURCES += bookmarks/BookmarkDialog.cpp \
           bookmarks/BookmarkModel.cpp \

HEADERS += cddb/Cddb.h \
           cddb/CddbPrivate.h \
           cddb/CddbSearchDialog.h \

SOURCES += cddb/Cddb.cpp \
           cddb/CddbPrivate.cpp \
           cddb/CddbSearchDialog.cpp \

HEADERS += AboutDialog.h
SOURCES += AboutDialog.cpp

