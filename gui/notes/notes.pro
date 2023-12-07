#-------------------------------------------------
#
# Project created by QtCreator 2017-05-05T05:17:02
#
#-------------------------------------------------

QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

ROOT = $$PWD/../..
LIBS_DIR = $$ROOT/poker_client/lib
include($$ROOT/config_qmake_all.pri)

localapp {
    TEMPLATE = app
#    SOURCES += main.cpp
    DESTDIR = $$ROOT/poker_client/bin/
} else {
    TEMPLATE = lib
#    CONFIG += staticlib
    DESTDIR = $$LIBS_DIR
}
static_build {
    DEFINES += STATIC_LIB
    CONFIG += staticlib
    LIBS += -lz -L$$LIBS_DIR -lbasewidgets_staticlib -lapi_models
}
dynamic_build {
    CONFIG(release, debug|release): LIBS += -lz -L$$LIBS_DIR -lbasewidgets -lapi_models
    CONFIG(debug, debug|release):   LIBS += -lz -L$$LIBS_DIR -lbasewidgetsd -lapi_modelsd
}

TARGET = notes
OBJECTS_DIR = ./tmp
MOC_DIR = ./tmp
UI_DIR = ./tmp
RCC_DIR = ./tmp

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += NOTES_LIB

#include($$PWD/../../plugins/widgets/widgets_include.pri)
INCLUDEPATH += $$PWD/.. \
               $$ROOT/tools \
               $$ROOT/basewidgets \
               $$ROOT/api_models
DEPENDPATH  += $$ROOT/basewidgets \
               $$ROOT/tools \
               $$ROOT/api_models

#TRANSLATIONS_PATH = $$ROOT/poker_client/data/translations
#TRANSLATIONS += $$TRANSLATIONS_PATH/notes_en.ts
#TRANSLATIONS += $$TRANSLATIONS_PATH/notes_ru.ts
#TRANSLATIONS += $$TRANSLATIONS_PATH/notes_uk.ts

SOURCES += \
        notespageview.cpp \
    notescompactview.cpp \
    noteslist.cpp \
    noteslisteditor.cpp \
    markerslist.cpp \
    markerslisteditor.cpp \
    model_notes.cpp \
    model_markers.cpp \
    data.cpp \
    notesview.cpp \
    delegates.cpp \
    model_io.cpp

HEADERS  += notespageview.h \
    notescompactview.h \
    noteslist.h \
    noteslisteditor.h \
    markerslisteditor.h \
    markerslist.h \
    model_notes.h \
    model_markers.h \
    data.h \
    notesview.h \
    delegates.h \
    notes_global.h \
    notes_global.h \
    plugins.h \
    model_io.h

FORMS    += notespageview.ui \
    notescompactview.ui \
    noteslist.ui \
    noteslisteditor.ui \
    markerslisteditor.ui \
    markerslist.ui
