
include(../../libs.pri)

TEMPLATE = lib
TARGET = $$ROOT/lib/coq_settings

FORMS   += $$ROOT/ui/SettingsPage1.ui \
           $$ROOT/ui/SettingsPage2.ui \
           $$ROOT/ui/SettingsPage3.ui \
           $$ROOT/ui/SettingsPage4.ui \

HEADERS += SettingsDialog.h \
           SettingsDialogPage.h \

SOURCES += SettingsDialog.cpp \
           SettingsDialogPage.cpp \
