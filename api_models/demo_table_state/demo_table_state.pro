#-------------------------------------------------
#
# Project created by QtCreator 2017-05-22T07:23:22
#
#-------------------------------------------------

QT += core gui widgets svg

DEFINES += GBP_DECLARE_TYPE_GEN_ADDITIONALS
TARGET = demo_table_state
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

DEFINES += STATIC_LIB
LIBS += -L$$LIBS_DIR -lcore# -lbasewidgets

SOURCES += main.cpp\
        tablestatedemo.cpp \
    ../../poker_client/src/core/money.cpp \
    ../../basewidgets/basewidgets/svgwidget.cpp \
    ../../basewidgets/basewidgets/entitywidget.cpp \
    ../../basewidgets/basewidgets/types.cpp \
    ../../basewidgets/basewidgets/util.cpp

HEADERS  += tablestatedemo.h \
    ../../basewidgets/basewidgets/entitywidget.h \
    ../../basewidgets/basewidgets/svgwidget.h \
    ../../basewidgets/basewidgets/types.h \
    ../../basewidgets/basewidgets/util.h

FORMS    += tablestatedemo.ui

