#include <precompiled.h>
#include "server_request_lobby.h"

#ifndef Q_MOC_RUN
    #include <netio/object_registry.hpp>
    #include "reply_lobby.h"
    #include "network_client.h"
#endif // QMOC_RUN

namespace ns_server_request
{


    special_offers::special_offers(NetworkClient * const client)
        : gbp::ns_api::ns_lobby::ns_server_request::special_offers()
        , m_reg(client->create_registry(parentClassName()))
        , m_client(*client)
        , m_emitter(client)
        , m_isInitialized(false)
    {
        init();
    }

    void special_offers::init()
    {
        if (!m_isInitialized)
        {
            m_reg->on("user_won_prize", this, &special_offers::user_won_prize);
            m_reg->on("show_soffer_random_prize_dialog", this, &special_offers::show_soffer_random_prize_dialog);
            m_isInitialized = true;
        }
    }

    const std::string &special_offers::parentClassName() {
        static const std::string name = "gbp::ns_api::ns_lobby::ns_server_request::special_offers";
        return name;
    }

    void special_offers::user_won_prize(const gbp::ns_api::user_prize &prize) {
        emit m_emitter.user_won_prize(prize);
    }

    void special_offers::show_soffer_random_prize_dialog() {
        emit m_emitter.show_soffer_random_prize_dialog();
    }

// --------------------LOBBY------------------------


    lobby::lobby(NetworkClient * const client)
        : gbp::ns_api::ns_lobby::ns_server_request::lobby()
        , m_reg(client->create_registry(parentClassName()))
        , m_client(*client)
        , m_emitter(client)
        , m_isInitialized(false)
    {
        init();
    }

    void lobby::init()
    {
        if (!m_isInitialized)
        {
            m_reg->on("server_stop", this, &lobby::server_stop);
            m_isInitialized = true;
        }
    }

    const std::string &lobby::parentClassName() {
        static const std::string name = "gbp::ns_api::ns_lobby::ns_server_request::lobby";
        return name;
    }

    void lobby::server_stop(gbp::ns_api::time_s date_time) {
        emit m_emitter.server_stop(date_time);
    }

} //namespace ns_server_request





