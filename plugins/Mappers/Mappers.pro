#CONFIG      += plugin release

CONFIG(release, debug|release): TARGET = mappersplugin
CONFIG(debug, debug|release):   TARGET = mappersplugind

TEMPLATE    = lib

ROOT = $$PWD/../..
include($$ROOT/config_qmake_all.pri)

INCLUDEPATH = $$ROOT/poker_client/src \
              $$ROOT/common \
              $$ROOT/tools \
              ../
DEPENDPATH =  $$ROOT/poker_client/src \
              $$ROOT/common
              $$ROOT/tools


HEADERS     = converttypemapperplugin.h intuintmapperplugin.h floatdoublemapperplugin.h transform2dmapperplugin.h transform3dmapperplugin.h mappers.h
SOURCES     = converttypemapperplugin.cpp intuintmapperplugin.cpp floatdoublemapperplugin.cpp transform2dmapperplugin.cpp transform3dmapperplugin.cpp mappers.cpp
#    ../util.cpp
RESOURCES   = icons.qrc
LIBS        += -L.

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += designer
} else {
    CONFIG += designer
}

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target
DESTDIR = $$PWD/../../poker_client/lib
include(transform2dmapper.pri)
include(intuintmapper.pri)
include(floatdoublemapper.pri)
include(converttypemapper.pri)
include(transform3dmapper.pri)

CONFIG += staticlib
staticlib {
    DEFINES += STATIC_LIB
}
