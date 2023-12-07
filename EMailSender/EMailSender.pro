QT       -= gui

TARGET = EMailSender
TEMPLATE = lib

ROOT = $$PWD/..
LIBS_DIR = $$ROOT/poker_client/lib
include($$ROOT/config_qmake_all.pri)

CONFIG += staticlib
OBJECTS_DIR = ./tmp
MOC_DIR = ./tmp
UI_DIR = ./tmp
RCC_DIR = ./tmp
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += emailsender.cpp

HEADERS += emailsender.h \
    ../SMTPEmail/src/SmtpMime
unix {
    target.path = /usr/lib
    INSTALLS += target
}
DESTDIR = $$PWD/../poker_client/lib/

LIBS += -L$$LIBS_DIR/ -lSMTPEmail #-lpython2.7

INCLUDEPATH += $$ROOT/SMTPEmail/src
DEPENDPATH += $$ROOT/SMTPEmail/src
INCLUDEPATH += $$PWD/../../../../mingw64/include
DEPENDPATH += $$PWD/../../../../mingw64/include
