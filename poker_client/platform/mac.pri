DEFINES += MAC_OS

#export DYLD_LIBRARY_PATH=/usr/lib/:$DYLD_LIBRARY_PATH
INCLUDEPATH += /opt/local/include \
              /usr/local/Cellar/libsodium/1.0.11/include \
              $$PWD/../common $$PWD/../common/netio/include \
              $$PWD/src
DEPENDPATH += /opt/local/include \
              /usr/local/Cellar/libsodium/1.0.11/include \
              $$PWD/lib

LIBS += -lz -L/usr/local/lib -lboost_system-mt
LIBS += -L/usr/local/Cellar/libsodium/1.0.11/lib/ -lsodium

DESTDIR = $$PWD/..
TARGET = ultima_poker
