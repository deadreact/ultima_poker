QT += core gui widgets svg
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++14 -Wall -Wextra

ROOT = $$PWD/../..
LIBS_DIR = $$ROOT/poker_client/lib

INCLUDEPATH += $$ROOT/animation $$ROOT/basewidgets
DEPENDPATH += $$ROOT/animation
LIBS += -L$$LIBS_DIR -lbasewidgets -lanimation


SOURCES += main.cpp \
    form.cpp

FORMS += \
    form.ui

HEADERS += \
    form.h \
    plugins.h

RESOURCES += \
    res.qrc
