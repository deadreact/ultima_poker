#-------------------------------------------------
#
# Project created by QtCreator 2017-04-20T23:34:03
#
#-------------------------------------------------
QT += widgets
TARGET = styles
TEMPLATE = lib

ROOT = $$PWD/../../..
LIBS_DIR = $$ROOT/poker_client/lib
include($$ROOT/config_qmake_all.pri)

INCLUDEPATH += $$ROOT/tools
DEPENDPATH += $$ROOT/tools


static_build {
    DEFINES += STATIC_LIB
    LIBS += -L$$LIBS_DIR -lcore -lbasewidgets_staticlib
    CONFIG += staticlib
}
dynamic_build {
CONFIG(release, debug|release): LIBS += -L$$LIBS_DIR -lcore -lbasewidgets
CONFIG(debug, debug|release): LIBS += -L$$LIBS_DIR -lcored -lbasewidgetsd
}


DESTDIR = $$LIBS_DIR
DEFINES += STYLES_LIB

MOC_DIR     = ./tmp/moc
UI_DIR      = ./tmp/ui
OBJECTS_DIR = ./tmp/obj
RCC_DIR     = ./tmp/rcc

#CONFIG += staticlib
staticlib {
    DEFINES += STATIC_LIB
}

INCLUDEPATH += $$ROOT/basewidgets
DEPENDPATH += $$ROOT/basewidgets

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    colorizedstyle.cpp \
    scaledtextstyle.cpp \
    shadowedtextstyle.cpp

HEADERS += \
    colorizedstyle.h \
    scaledtextstyle.h \
    shadowedtextstyle.h \
    styles_global.h
