#pragma once

#ifndef Q_MOC_RUN
    #include <api/lobby/server_request.h>
#endif // Q_MOC_RUN

#include <QObject>
#include <memory>
#include <tools/types_fwd.hpp>

class NetworkClient;

namespace ns_server_request
{
    class emitter_lobby : public QObject
    {
        Q_OBJECT
    public:
        explicit emitter_lobby(QObject *parent = 0) : QObject(parent) {}
    signals:
        void user_won_prize(const gbp::ns_api::user_prize &prize);
        void show_soffer_random_prize_dialog();
        void server_stop(gbp::ns_api::time_s date_time);
    };

    struct special_offers : gbp::ns_api::ns_lobby::ns_server_request::special_offers
    {
        using emitter_t = emitter_lobby;
    private:
        std::shared_ptr<gbp::net::object_registry> m_reg;
        NetworkClient& m_client;
        emitter_t m_emitter;
        bool m_isInitialized;
    public:
        special_offers(NetworkClient * const client);

        const emitter_t* emitter() const { return &m_emitter; }
        void init();

        static const std::string& parentClassName();

        virtual void user_won_prize(const gbp::ns_api::user_prize &prize) override;
        virtual void show_soffer_random_prize_dialog() override;
    };

    struct lobby : gbp::ns_api::ns_lobby::ns_server_request::lobby
    {
        using emitter_t = emitter_lobby;
    private:
        std::shared_ptr<gbp::net::object_registry> m_reg;
        NetworkClient& m_client;
        emitter_t m_emitter;
        bool m_isInitialized;
    public:
        lobby(NetworkClient * const client);

        const emitter_t* emitter() const { return &m_emitter; }
        void init();

        static const std::string& parentClassName();

        virtual void server_stop(gbp::ns_api::time_s date_time) override;
    };

} //namespace ns_server_request

