
INCLUDEPATH += $$PWD/src $$PWD/../common $$PWD/../common/netio/include
DEPENDPATH += $$PWD/src $$PWD/../common $$PWD/../common/netio/include

LIBS += -lz \
        -lboost_system \
        -lboost_filesystem \
        -lsodium

DESTDIR = $$PWD/..
TARGET = ultima_poker


RESOURCES_DIR = $$PWD/../../resources
system(echo compiling resource package...)
system(cd $$RESOURCES_DIR/unpacked && rcc -o tmp.qrc --project && rcc --binary -o $$DESTDIR/bin/resources.dat tmp.qrc && rm tmp.qrc)

static_build {
    DEFINES += NO_DLL_IMPORT
    CONFIG(release, debug|release): LIBS += -L$$PWD/../lib -ltools_staticlib -lrpcbase -lbasewidgets_staticlib -lmappersplugin -lcore -lstyles -lapi_models -lnotes -lEMailSender -lSMTPEmail -lcashier_staticlib -lanimation_staticlib
    CONFIG(debug, debug|release): LIBS += -L$$PWD/../lib -ltools_staticlib -lrpcbase   -lbasewidgets_staticlib -lmappersplugind -lcored -lstyles -lapi_modelsd -lnotes -lEMailSender -lSMTPEmail -lcashier_staticlib -lanimation_staticlib
}
dynamic_build {
    CONFIG(release, debug|release): LIBS += -L$$PWD/../lib -ltools -lrpcbase -lbasewidgets -lmappersplugin -lcore -lstyles -lapi_models -lnotes -lEMailSender -lSMTPEmail -lcashier_staticlib -lanimation
    CONFIG(debug, debug|release): LIBS += -L$$PWD/../lib -ltoolsd -lrpcbase -lbasewidgetsd -lmappersplugind -lcored -lstyles -lapi_modelsd -lnotes -lEMailSender -lSMTPEmail -lcashier_staticlib -lanimationd
}
