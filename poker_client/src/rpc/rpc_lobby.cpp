#include <precompiled.h>
#include "rpc_lobby.h"
#include <basewidgets/util.h>
#include <qtimer.h>

#ifndef Q_MOC_RUN
    #include "request_lobby.h"
    #include "network_client.h"
#endif // Q_MOC_RUN

namespace
{
    void killConnection(QMetaObject::Connection* conn) {
        if (conn != nullptr) {
            QObject::disconnect(*conn);
            delete conn;
        }
    }
}

rpc_lobby::rpc_lobby(NetworkClient * const client)
    : ns_lobby::rpc_object()
    , m_request(client->request<request_lobby>())
    , m_emitter(client->emitter<reply_lobby>())
{

}

void rpc_lobby::user_avatar(on_get_avatar callback)
{
    if (netClient().userId() == -1) {
        if (callback) {
            QTimer::singleShot(0, Qt::CoarseTimer, [callback]{callback(ns_lobby::e_status::unknown_error, std::string());});
        }
        return;
    }
    if (callback)
    {
        QMetaObject::Connection* conn = new QMetaObject::Connection;
        auto callback2 = [callback, conn](ns_lobby::e_status status, const std::unordered_map<gbp_i64, std::string>& avatars) {
            auto it = avatars.find(netClient().userId());
            if (it != avatars.end()) {
                killConnection(conn);
                callback(status, it->second);
            }
        };
        *conn = QObject::connect(m_emitter, &reply_lobby::emitter_t::get_avatars, callback2);
    }
    m_request->get_avatar(netClient().userId());
}


gbp::ns_api::ns_lobby::ns_request::lobby_common *rpc_lobby::request_common() const {
    return m_request;
}

gbp::ns_api::ns_lobby::ns_request::lobby_desktop *rpc_lobby::request_desktop() const {
    return m_request;
}

const ns_lobby::emitter_t *rpc_lobby::emitter() const {
    return m_emitter;
}
