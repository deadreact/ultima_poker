QT += core gui widgets svg

TEMPLATE = lib
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += CASHIER_LIB

ROOT = $$PWD/../../..
LIBS_DIR = $$ROOT/poker_client/lib
include($$ROOT/config_qmake_all.pri)

unix: CONFIG += static_build

INCLUDEPATH += $$PWD/.. \
               $$ROOT/tools \
               $$ROOT/common \
               $$ROOT/poker_client/src \
               $$ROOT/basewidgets \
               $$ROOT/api_models \
               $$ROOT/animation
DEPENDPATH  += $$PWD/.. \
               $$ROOT/tools \
               $$ROOT/common \
               $$ROOT/poker_client/src \
               $$ROOT/basewidgets \
               $$ROOT/api_models \
               $$ROOT/animation
DESTDIR = $$LIBS_DIR

static_build {
    LIBS += -L$$LIBS_DIR -lrpcbase -lcore -lanimation_staticlib -lapi_models -lbasewidgets_staticlib
    DEFINES += STATIC_LIB
    TARGET = cashier_staticlib
    CONFIG += staticlib
}
else {
dynamic_build {
    CONFIG(release, debug|release): LIBS += -L$$LIBS_DIR -lrpcbase -lanimation -lcore -lbasewidgets -lapi_models
    CONFIG(debug, debug|release): LIBS += -L$$LIBS_DIR -lrpcbase -lanimationd -lcored -lbasewidgetsd -lapi_modelsd
    CONFIG(debug, debug|release): TARGET = cashierd
    CONFIG(release, debug|release): TARGET = cashier
}
}




#localapp {
#    TEMPLATE = app
#    DESTDIR = $$ROOT/poker_client/bin/
#} else {
#    TEMPLATE = lib
#    CONFIG += staticlib
#    DEFINES += CASHIER_LIB
#    DESTDIR = $$LIBS_DIR
#}

SOURCES +=\
        cashierdialog.cpp \
    page_cashierbase.cpp \
    page_deposit.cpp \
    page_pendingwithdrawals.cpp \
    page_transactions.cpp \
    page_withdrawal.cpp \
    model_pendingwithdrawals.cpp

HEADERS  += cashierdialog.h \
    cashier_global.h \
    page_cashierbase.h \
    page_deposit.h \
    page_pendingwithdrawals.h \
    page_transactions.h \
    page_withdrawal.h \
    model_pendingwithdrawals.h \
    plugins.h

FORMS    += cashierdialog.ui \
    page_deposit.ui \
    page_pendingwithdrawals.ui \
    page_transactions.ui \
    page_withdrawal.ui




#win32:CONFIG(release, debug|release): LIBS += -L$$LIBS_DIR/ -lbasewidgets_staticlib
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$LIBS_DIR/ -lbasewidgets_staticlib
#else:unix: LIBS += -L$$LIBS_DIR/ -lbasewidgets_staticlib

#INCLUDEPATH += $$PWD/../../../basewidgets/basewidgets
#DEPENDPATH += $$PWD/../../../basewidgets/basewidgets

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIBS_DIR/libbasewidgets_staticlib.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIBS_DIR/libbasewidgets_staticlib.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIBS_DIR/basewidgets_staticlib.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIBS_DIR/basewidgets_staticlib.lib
#else:unix: PRE_TARGETDEPS += $$LIBS_DIR/libbasewidgets_staticlib.a
