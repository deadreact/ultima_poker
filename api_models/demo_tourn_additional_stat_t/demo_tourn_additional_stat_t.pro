#-------------------------------------------------
#
# Project created by QtCreator 2017-05-30T18:07:48
#
#-------------------------------------------------

QT += core gui widgets svg


DEFINES += GBP_DECLARE_TYPE_GEN_ADDITIONALS
TARGET = demo_tourn_additional_stat_t
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11

OBJECTS_DIR = ./tmp
MOC_DIR = ./tmp
UI_DIR = ./tmp
RCC_DIR = ./tmp

ROOT = $$PWD/../..
LIBS_DIR = $$ROOT/poker_client/lib

include(../api_models/api_models.pri)

INCLUDEPATH += $$ROOT/common \
               $$ROOT/basewidgets \
               $$ROOT/poker_client/src/core \
               $$PWD/../api_models
DEPENDPATH  += $$ROOT/common \
               $$ROOT/basewidgets \
               $$ROOT/poker_client/src/core \
               $$PWD/../api_models

LIBS += -L$$LIBS_DIR -lcore -lbasewidgets_staticlib
static_build {
    DEFINES += NO_DLL_IMPORT
}

SOURCES += main.cpp\
        widget.cpp \
    ../../basewidgets/basewidgets/types.cpp \
    ../../basewidgets/basewidgets/util.cpp

HEADERS  += widget.h

FORMS    += widget.ui
