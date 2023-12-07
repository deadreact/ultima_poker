QT += core gui widgets
TARGET = animation

ROOT = $$PWD/../..
LIBS_DIR = $$ROOT/poker_client/lib
include($$ROOT/config_qmake_all.pri)

INCLUDEPATH += $$ROOT/basewidgets \
               $$ROOT/tools
DEPENDPATH  += $$ROOT/basewidgets \
               $$ROOT/tools
TEMPLATE = lib

dynamic_build {
    CONFIG(debug, debug|release):   LIBS += -L$$LIBS_DIR -lbasewidgetsd
    CONFIG(release, debug|release): LIBS += -L$$LIBS_DIR -lbasewidgets
    CONFIG(debug, debug|release): TARGET = animationd
    CONFIG(release, debug|release): TARGET = animation
}
static_build {
    LIBS += -L$$LIBS_DIR -lbasewidgets_staticlib
    DEFINES += staticlib
    DEFINES += STATIC_LIB
    TARGET = animation_staticlib
}

DESTDIR = $$LIBS_DIR

DEFINES += ANIMATION_LIB
DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += animation.cpp \
    rotatedwidget.cpp

HEADERS += animation.h\
        animation_global.h \
    rotatedwidget.h

unix {
    target.path = $$[QT_INSTALL_LIBS]
    INSTALLS += target
}

#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../basewidgets/basewidgets/release/ -lbasewidgets
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../basewidgets/basewidgets/debug/ -lbasewidgets
#else:unix: LIBS += -L$$OUT_PWD/../../basewidgets/basewidgets/ -lbasewidgets

INCLUDEPATH += $$PWD/../../basewidgets/basewidgets
DEPENDPATH += $$PWD/../../basewidgets/basewidgets
