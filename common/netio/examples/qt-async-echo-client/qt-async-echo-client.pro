QT += core
QT -= gui

TARGET = qt-async-echo-client
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += \
    -std=c++14

DEFINES += \
    QT_DEPRECATED_WARNINGS

INCLUDEPATH += \
    . \
    ../../include

LIBS += \
    -lboost_system \
    -lz \
    -lsodium \
    -lpthread

SOURCES += \
    main.cpp \
    ../../src/cryptotools.cpp \
    ../../src/distributor.cpp \
    ../../src/netio.cpp \
    ../../src/object_registry.cpp \
    ../../src/profdata.cpp \
    ../../src/qt_netio.cpp \
    ../../src/service_handle.cpp
