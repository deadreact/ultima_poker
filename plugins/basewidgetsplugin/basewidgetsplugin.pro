QT += uitools designer widgets svg
CONFIG += debug_and_release plugin c++11
TARGET      = basewidgetsplugin
TARGET      = $$qtLibraryTarget($$TARGET)
TEMPLATE    = lib

ROOT = $$PWD/../..
LIBS_DIR = $$ROOT/poker_client/lib
message($$OUT_PWD/plugins_base/release)

static_build {
LIBS +=  -L$$PWD/.. -lplugins_base -L$$LIBS_DIR -lbasewidgets_staticlib -lanimation_staticlib -ltools_staticlib
} else {
CONFIG(release, debug|release): LIBS += -L$$PWD/.. -lplugins_base -L$$LIBS_DIR -lbasewidgets -lanimation -ltools
CONFIG(debug, debug|release): LIBS += -L$$PWD/.. -lplugins_base -L$$LIBS_DIR -lbasewidgetsd -lanimationd -ltoolsd
}
INCLUDEPATH += $$ROOT/basewidgets \
               $$ROOT/animation \
               $$ROOT/tools
DEPENDPATH  += $$ROOT/basewidgets \
               $$ROOT/animation
               $$ROOT/tools

RESOURCES   = icons.qrc
DEFINES += DESIGNER

OBJECTS_DIR = ./tmp
MOC_DIR = ./tmp
UI_DIR = ./tmp
RCC_DIR = ./tmp

#message($$[QT_INSTALL_LIBS])

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target

DISTFILES += \
    ../pluginsData.db

FORMS += \
    rttwidget.ui

HEADERS += \
    correctinputplugin.h \
    customwidgets.h \
    plugin_common.h \
    rttwidget.h \
    rttwidgetplugin.h

SOURCES += \
    correctinputplugin.cpp \
    customwidgets.cpp \
    plugin_common.cpp \
    rttwidget.cpp \
    rttwidgetplugin.cpp
