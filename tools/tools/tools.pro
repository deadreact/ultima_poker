QT += core gui widgets

DEFINES += TOOLS_LIB
TEMPLATE = lib

ROOT = $$PWD/../..
LIBS_DIR = $$ROOT/poker_client/lib
include($$ROOT/config_qmake_all.pri)

#!contains(DEFINES, DESIGNER) {
#    CONFIG += static_build
#}

dynamic_build {
    CONFIG(debug, debug|release): TARGET = toolsd
    CONFIG(release, debug|release): TARGET = tools
}
static_build {
    CONFIG += staticlib
    DEFINES += STATIC_LIB
    TARGET = tools_staticlib
}

OBJECTS_DIR = ./tmp/obj
MOC_DIR     = ./tmp/moc
UI_DIR      = ./tmp/ui
RCC_DIR     = ./tmp/rcc



DESTDIR = $$LIBS_DIR
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += $$ROOT/common \
               $$PWD/..
DEPENDPATH += $$ROOT/common

include(tools.pri)


#message($$[QT_INSTALL_PLUGINS])
#target.path = $$[QT_INSTALL_BINS]
#INSTALLS    += target
#}

