QT += gui core widgets

ROOT = $$PWD/../..
LIBS_DIR = $$ROOT/poker_client/lib
include($$ROOT/config_qmake_all.pri)

TEMPLATE = lib
DEFINES += API_MODELS_LIB
#CONFIG += shared
CONFIG += staticlib
DEFINES += STATIC_LIB
#shared {
#    DEFINES += API_MODELS_SHARED_LIB
#}
CONFIG(debug, debug|release): TARGET = api_modelsd
CONFIG(release, debug|release): TARGET = api_models
#staticlib {
#    TARGET = api_models
#}

DESTDIR = $$LIBS_DIR
#win32: DLLDESTDIR = $$ROOT/poker_client

INCLUDEPATH += $$ROOT/common \
               $$ROOT/common/rpcbase \
               $$ROOT/basewidgets \
               $$ROOT/poker_client/src/core \
               $$ROOT/tools
DEPENDPATH  += $$ROOT/common \
               $$ROOT/common/rpcbase \
               $$ROOT/basewidgets \
               $$ROOT/tools

CONFIG(debug, debug|release): LIBS += -L$$LIBS_DIR -lcored -lrpcbase -ltoolsd
CONFIG(release, debug|release): LIBS += -L$$LIBS_DIR -lcore -lrpcbase -ltools


DEFINES += QT_DEPRECATED_WARNINGS

include (api_models.pri)

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    icons.qrc

#SOURCES += \
#    ../../basewidgets/basewidgets/types.cpp

#HEADERS += \
#    ../../basewidgets/basewidgets/types.h
