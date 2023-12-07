#pragma once


#ifndef Q_MOC_RUN
    #include <rpcbase/rpcbase_lobby.h>
    #include "reply_lobby.h"
#endif // Q_MOC_RUN

#include <core/money.h>

class request_lobby;
class NetworkClient;


struct rpc_lobby : ns_lobby::rpc_object
{
private:
    request_lobby* m_request;
    const reply_lobby::emitter_t* m_emitter;
public:
    explicit rpc_lobby(NetworkClient * const client);

    using on_get_avatar = std::function<void(ns_lobby::e_status, const std::string&)>;
    void user_avatar(on_get_avatar callback);

public:
    virtual gbp::ns_api::ns_lobby::ns_request::lobby_common *request_common() const override;
    virtual gbp::ns_api::ns_lobby::ns_request::lobby_desktop *request_desktop() const override;
    virtual const ns_lobby::emitter_t *emitter() const override;
};

