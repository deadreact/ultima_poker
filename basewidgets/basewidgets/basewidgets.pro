QT += core gui widgets svg

TEMPLATE = lib
CONFIG += debug_and_release

ROOT = $$PWD/../..
LIBS_DIR = $$ROOT/poker_client/lib
include($$ROOT/config_qmake_all.pri)

dynamic_build {
    CONFIG(debug, debug|release): TARGET = basewidgetsd
    CONFIG(release, debug|release): TARGET = basewidgets
}
static_build{
    CONFIG += staticlib
    DEFINES += STATIC_LIB
    TARGET = basewidgets_staticlib
}

OBJECTS_DIR = ./tmp/obj
MOC_DIR     = ./tmp/moc
UI_DIR      = ./tmp/ui
RCC_DIR     = ./tmp/rcc


DESTDIR = $$LIBS_DIR
DEFINES += BASEWIDGETS_LIB
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += $$ROOT/tools
DEPENDPATH += $$ROOT/tools

SOURCES += \
    clockwidget.cpp \
    correctinput.cpp \
    country_combobox.inl \
    countryinfomodel.cpp \
    easingCurveComboBox.cpp \
    entitywidget.cpp \
    framedbutton.cpp \
    framedentity.cpp \
    infomessage.cpp \
    logic.cpp \
    svgwidget.cpp \
    textwidget.cpp \
    timezonebox.cpp \
    validatedLineEdit.cpp \
    models/model_timezone.cpp \
    types.cpp \
    util.cpp

HEADERS +=\
        basewidgets_global.h \
    clockwidget.h \
    correctinput.h \
    countryinfomodel.h \
    easingCurveComboBox.h \
    entitywidget.h \
    framedbutton.h \
    framedentity.h \
    infomessage.h \
    logic.h \
    svgwidget.h \
    textwidget.h \
    timezonebox.h \
    validatedLineEdit.h \
    models/model_timezone.h \
    types.h \
    util.h

unix {
    target.path = $$[QT_INSTALL_LIBS]
    INSTALLS += target
}
