QT += core gui widgets svg
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++14 -Wall -Wextra

ROOT = $$PWD/../../..
LIBS_DIR = $$ROOT/poker_client/lib

INCLUDEPATH += $$PWD/../cashier
DEPENDPATH  += $$PWD/../cashier
DEPENDPATH  += $$LIBS_DIR

LIBS += -L$$LIBS_DIR -lcashier.dll -lbasewidgets -lapi_models
SOURCES += \
    main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../poker_client/lib/ -llibbasewidgets.dll
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../poker_client/lib/ -llibbasewidgets.dlld
else:unix: LIBS += -L$$PWD/../../../poker_client/lib/ -llibbasewidgets.dll

ROOT = $$PWD/../../..

INCLUDEPATH += $$ROOT/poker_client \
               $$ROOT/api_models \
               $$ROOT/common \
               $$ROOT/basewidgets
DEPENDPATH += $$ROOT/poker_client \
               $$ROOT/api_models \
               $$ROOT/common \
               $$ROOT/basewidgets
