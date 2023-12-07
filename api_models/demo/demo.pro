QT       += core gui widgets sql svg

TARGET = demo
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11

OBJECTS_DIR = ./tmp
MOC_DIR = ./tmp
UI_DIR = ./tmp
RCC_DIR = ./tmp
DEFINES += GBP_DECLARE_TYPE_GEN_ADDITIONALS
ROOT = $$PWD/../..
LIBS_DIR = $$ROOT/poker_client/lib

INCLUDEPATH += $$ROOT/common \
               $$ROOT/basewidgets
DEPENDPATH  += $$ROOT/common \
               $$ROOT/basewidgets
include(../api_models/api_models.pri)
#DEFINES += STATIC_LIB
LIBS += -L$$LIBS_DIR -lbasewidgetsd
INCLUDEPATH += $$ROOT/poker_client/src
DEPENDPATH +=  $$ROOT/poker_client/src/core

SOURCES += main.cpp \
    form.cpp

HEADERS  += \
    form.h \
    plugins.h

FORMS += \
    form.ui

INCLUDEPATH += $$PWD/../api_models
DEPENDPATH += $$PWD/../api_models

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../poker_client/lib/ -lcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../poker_client/lib/ -lcored
else:unix: LIBS += -L$$PWD/../../poker_client/lib/ -lcore

INCLUDEPATH += $$PWD/../../poker_client/src/core
DEPENDPATH += $$PWD/../../poker_client/src/core

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../poker_client/lib/libcore.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../poker_client/lib/libcored.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../poker_client/lib/core.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../poker_client/lib/cored.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../poker_client/lib/libcore.a
