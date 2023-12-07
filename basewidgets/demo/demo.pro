#-------------------------------------------------
#
# Project created by QtCreator 2017-05-17T03:00:00
#
#-------------------------------------------------

QT += core gui widgets svg
CONFIG += c++11
TARGET = demo
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += main.cpp\
        widget.cpp
HEADERS  += widget.h \
    plugins.h
FORMS    += widget.ui

OBJECTS_DIR = ./tmp/obj
MOC_DIR     = ./tmp/moc
UI_DIR      = ./tmp/ui
RCC_DIR     = ./tmp/rcc

ROOT = $$PWD/../..
LIBS_DIR = $$ROOT/poker_client/lib

CONFIG(release, debug|release): LIBS += -L$$LIBS_DIR -lbasewidgets
CONFIG(debug, debug|release):   LIBS += -L$$LIBS_DIR -lbasewidgetsd
INCLUDEPATH += $$PWD/../basewidgets
DEPENDPATH += $$PWD/../basewidgets
