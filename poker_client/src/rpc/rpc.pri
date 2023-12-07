
NETIO_PRI = $$PWD/../../../common/
include($$NETIO_PRI/netio.pri)
NETIO_SRC = $$NETIO_PRI/netio/src
NETIO_INCL = $$NETIO_PRI/netio/include

INCLUDEPATH += $$PWD/ \
               $$NETIO_INCL
DEPENDPATH += $$NETIO_INCL $$NETIO_SRC

HEADERS +=  \
            $$PWD/network_client.h \
            $$PWD/reply_lobby.h \
            $$PWD/request_lobby.h \
            $$PWD/rpc_lobby.h \
            $$PWD/server_request_lobby.h \
            $$PWD/table/reply.h \
            $$PWD/table/request.h \
            $$PWD/table/server_reply.h \
            $$PWD/table/server_request.h \
    $$PWD/rpc_common.h \
    $$PWD/table/rpc_table.h \
    $$PWD/rpc_statistic.h \
    $$PWD/reply_statistic.h \
    $$PWD/request_statistic.h

SOURCES += \
    $$PWD/network_client.cpp \
    $$PWD/reply_lobby.cpp \
    $$PWD/request_lobby.cpp \
    $$PWD/rpc_lobby.cpp \
    $$PWD/server_request_lobby.cpp \
    $$PWD/table/reply.cpp \
    $$PWD/table/request.cpp \
    $$PWD/table/server_reply.cpp \
    $$PWD/table/server_request.cpp \
    $$PWD/rpc_common.cpp \
    $$PWD/table/rpc_table.cpp \
    $$PWD/rpc_statistic.cpp \
    $$PWD/reply_statistic.cpp \
    $$PWD/request_statistic.cpp

