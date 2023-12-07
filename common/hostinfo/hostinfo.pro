
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_LFLAGS += \
    -pthread \
    -Wall \
    -Wextra

SOURCES += \
    main.cpp \
    hostinfo.cpp

LIBS += \
    -L/usr/local/lib \
    -lboost_system \
    -lboost_filesystem

win32: {
    LIBS += \
        -lws2_32 \
        -liphlpapi \
        -lnetapi32
}
