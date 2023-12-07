QT       -= gui

TEMPLATE = lib

ROOT = $$PWD/../..
LIBS_DIR = $$ROOT/poker_client/lib
include($$ROOT/config_qmake_all.pri)

include(../api.pri)
#include(../netio.pri)

#CONFIG += shared
CONFIG += staticlib

INCLUDEPATH += $$PWD/..
DEPENDPATH  += $$PWD/..
INCLUDEPATH += $$ROOT/tools
DEPENDPATH += $$ROOT/tools

shared {
    CONFIG(debug, debug|release): TARGET = rpcbased
    CONFIG(release, debug|release): TARGET = rpcbase
    CONFIG(debug, debug|release): LIBS += -L$$LIBS_DIR -ltoolsd
    CONFIG(release, debug|release): LIBS += -L$$LIBS_DIR -ltools
}
staticlib {
    DEFINES += STATIC_LIB
    TARGET = rpcbase
    LIBS += -L$$LIBS_DIR -ltools_staticlib
}

OBJECTS_DIR = ./tmp/obj
MOC_DIR     = ./tmp/moc
UI_DIR      = ./tmp/ui
RCC_DIR     = ./tmp/rcc


DESTDIR = $$LIBS_DIR
DEFINES += RPCBASE_LIB
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    emitter_lobby.cpp \
    rpcbase_lobby.cpp \
    emitter_table.cpp \
    rpcbase_table.cpp \
    emitter_statistic.cpp \
    rpcbase_statistic.cpp

HEADERS += \
    emitter_lobby.h \
    rpcbase_lobby.h \
    emitter_table.h \
    rpcbase_table.h \
    defs.h \
    emitter_statistic.h \
    rpcbase_statistic.h \

unix {
    target.path = /usr/lib
    INSTALLS += target
}
