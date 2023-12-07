QT += winextras
DEFINES += WIN_OS

RC_FILE = $$PWD/../../resources/unpacked/poker_client.rc

INCLUDEPATH += $$PWD/src \
               $$PWD/lib \
               $$PWD/../common \
               $$PWD/../common/netio/include
DEPENDPATH += $$PWD/src \
              $$PWD/lib \
              $$PWD/../common \
              $$PWD/../common/netio/include

LIBS += -lz \
        -lboost_system-mt \
        -lboost_filesystem-mt \
        -lsodium -lws2_32 \
        -lole32 \
        -lnetapi32 \
        -lws2_32 \
        -liphlpapi

win32-g++:contains(QMAKE_HOST.arch, x86_64):{
   TARGET = UltimaPoker_x64
}

DESTDIR = $$PWD/..

win32:CONFIG(debug, debug|release): DEFINES += _DEBUG
win32: CONFIG += remote
win32 {
        CONFIG(release, debug|release): TARGET = ultima_poker
        CONFIG(debug, debug|release):   TARGET = ultima_poker
        remote {
                DEFINES += REMOTE
                DEFINES += VM_PROTECT
                DEFINES += RESOURCES_IN_QT_RESOURCES_FILE
        }
        RESOURCES_DIR = $$PWD/../../resources
        system(echo compiling resource package...)
        system(cd $$RESOURCES_DIR/unpacked && rcc -o tmp.qrc --project && rcc --binary -o $$DESTDIR/bin/resources.dat tmp.qrc && del tmp.qrc)
}

static_build {
    DEFINES += NO_DLL_IMPORT
    CONFIG(release, debug|release): LIBS += -L$$PWD/../lib -ltools_staticlib -lrpcbase -lbasewidgets_staticlib -lmappersplugin -lcore -lstyles -lapi_models -lnotes -lEMailSender -lSMTPEmail -lcashier_staticlib -lanimation_staticlib
    CONFIG(debug, debug|release): LIBS += -L$$PWD/../lib -ltools_staticlib -lrpcbase   -lbasewidgets_staticlib -lmappersplugind -lcored -lstyles -lapi_modelsd -lnotes -lEMailSender -lSMTPEmail -lcashier_staticlib -lanimation_staticlib
}
dynamic_build {
    CONFIG(release, debug|release): LIBS += -L$$PWD/../lib -ltools -lrpcbase -lbasewidgets -lmappersplugin -lcore -lstyles -lapi_models -lnotes -lEMailSender -lSMTPEmail -lcashier -lanimation
    CONFIG(debug, debug|release): LIBS += -L$$PWD/../lib -ltoolsd -lrpcbase -lbasewidgetsd -lmappersplugind -lcored -lstyles -lapi_modelsd -lnotes -lEMailSender -lSMTPEmail -lcashierd -lanimationd
}
