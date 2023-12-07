
#include <netio/netio.hpp>

#include "../services/ping_service.hpp"
#include "../services/remote_time_service.hpp"
#include "../services/disconnect_service.hpp"

#include <iostream>

#include <boost/tti/has_member_function.hpp>

/*********************************************************************************************************************/

struct session: std::enable_shared_from_this<session> {
    session(gbp::net::socket_type sock)
        :m_netio(
             sock
            ,gbp::net::netio::server|gbp::net::netio::test_inactivity
            ,10 // inactivity timeout
            ,gbp::net::netio::k_max_body_size
            ,100 // compress if greater
            ,[this](gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg) {
                (void)holder;

                std::cout << "error_handler: " << fl << ": ec=" << ec << ", emsg=" << emsg << std::endl;

                m_prof_timer->stop();
            }
            ,[this](gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg, char *, std::size_t) {
                (void)holder;

                if ( ec ) {
                    std::cout << "read_handler: " << fl << ": ec=" << ec << ", emsg=" << emsg << std::endl;
                }
            }
            ,[this](gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg, const char *, std::size_t) {
                (void)holder;

                if ( ec ) {
                    std::cout << "write_handler: " << fl << ": ec=" << ec << ", emsg=" << emsg << std::endl;
                }
            }
        )
        ,m_prof_timer{gbp::net::create_one_shot_timer(m_netio.get_worker())}
    {
        std::cout << "session ctor:" << this << std::endl;

//        m_netio.set_incoming_handler(
//            [this]
//            (gbp::net::impl_holder holder, const char *cls, const char *func, const char */*buf*/, std::size_t /*size*/)
//            { std::cout << "incoming_handler: " << cls << "." << func << std::endl; }
//        );
    }

    virtual ~session() {
        std::cout << "session dtor:" << this << std::endl;
    }

    void start(gbp::net::impl_holder holder) {
        m_netio.add_service<gbp::net::ping_service>(
             1 // ping interval
            ,[](std::uint64_t time_us){ std::cout << "ping_time=" << time_us << "us" << std::endl; }
        );
        m_netio.add_service<gbp::net::remote_time_service>();
        m_netio.add_service<gbp::net::disconnect_service>();

        if ( m_netio.is_no_crypt() ) {
            m_netio.start(std::move(holder));
        } else {
            start_handshake(std::move(holder));
        }
    }

    void start_handshake(gbp::net::impl_holder holder) {
        m_netio.async_handshake(
            [this](gbp::net::impl_holder holder, const char *fileline, int ec, const char *emsg)
            { on_handshake(std::move(holder), fileline, ec, emsg); }
            ,std::move(holder)
        );
    }

    void on_handshake(gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg) {
        if ( ec ) {
            std::cout << "handshake_handler: " << fl << ", ec=" << ec << ", emsg=" << emsg << std::endl;
            return;
        }

        std::cout << "shared key: obj=" << this << ", key=" << gbp::net::hexstring(m_netio.shared_key(), m_netio.shared_key_size()) << std::endl;

        start_timer(holder);
        m_netio.start(holder);

        {
            auto s = m_netio.get_service<gbp::net::remote_time_service>();
            assert(s);
            s->get_time([this](std::uint64_t time) { std::cout << "remote time=" << time << std::endl; });
        }
        {
            auto s = m_netio.get_service<gbp::net::disconnect_service>();
            assert(s);
            s->set_disconnect_handler(
                [this](std::int32_t ec) {
                    std::cout << "disconnect_service::disconnect_handler: disconnect requested, reason=" << ec << std::endl;
//                    m_prof_timer->stop();
//                    m_netio.stop();
                }
            );
        }
    }

    void start_timer(gbp::net::impl_holder holder) {
        m_prof_timer->start(
             5000
            ,[this](gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg)
             { on_timeout(std::move(holder), fl, ec, emsg); }
            ,std::move(holder)
        );
    }
    void on_timeout(gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg) {
        if ( ec == gbp::net::error::operation_aborted )
            return;

        if ( ec ) {
            std::cout << "on_timeout: " << fl << ": ec=" << ec << ", emsg=" << emsg << std::endl;

            return;
        }

        auto cb = [](const std::string &object, const std::string &proc, const gbp::net::profdata &data) {
            std::cout << "profdata: " << object << "." << proc << ": " << data.dump() << std::endl;
        };

        m_netio.get_profdata(cb);

        start_timer(std::move(holder));
    }

private:
    gbp::net::netio m_netio;
    gbp::net::timer_type m_prof_timer;
};

/*********************************************************************************************************************/

struct server {
    server(gbp::net::worker &w, const char *ip, std::uint16_t port)
        :acceptor{gbp::net::create_acceptor(w, ip, port)}
    {}

    void start(gbp::net::impl_holder holder = gbp::net::impl_holder()) {
        acceptor->async_accept(
            [this](gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg, gbp::net::socket_type sock)
            { on_accepted(std::move(holder), fl, ec, emsg, std::move(sock)); }
            ,std::move(holder)
        );
    }

private:
    void on_accepted(gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg, gbp::net::socket_type sock) {
        if (ec) {
            std::cout << "accept error(" << fl << ": ec=" << ec << "): " << emsg << std::endl;
            return;
        }

        auto sesp = std::make_shared<session>(std::move(sock));
        sesp->start(sesp->shared_from_this());

        start(std::move(holder));
    }

    gbp::net::acceptor_type acceptor;
};

/*********************************************************************************************************************/

int main() {
    gbp::net::worker worker;

    auto ip = "127.0.0.1";
    auto port = 1234;
    server serv(worker, ip, port);
    serv.start();

    worker.run();

    return 0;
}

/*********************************************************************************************************************/
