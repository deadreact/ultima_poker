QT += core gui widgets
CONFIG      += release
#TARGET      = $$qtLibraryTarget(mappersplugin)
TEMPLATE    = lib
CONFIG += staticlib
DESTDIR = $$PWD/..

DEFINES += DESIGNER
INCLUDEPATH += ../
#HEADERS += ../types.h

LIBS        += -L.
QMAKE_CXXFLAGS += -std=c++14

HEADERS += pluginhelper.h
SOURCES += pluginhelper.cpp

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += designer
} else {
    CONFIG += designer
}

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target
