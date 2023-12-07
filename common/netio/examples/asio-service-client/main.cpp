
#include <netio/netio.hpp>

#include "../services/ping_service.hpp"
#include "../services/remote_time_service.hpp"
#include "../services/disconnect_service.hpp"

#include <iostream>

/*********************************************************************************************************************/

struct client {
    client(gbp::net::worker &w, const char *ip, const std::uint16_t port)
        :m_netio(
             w
            ,gbp::net::netio::client
            ,10 // inactivity timeout
            ,gbp::net::netio::k_max_body_size
            ,100 // compress if greater
            ,[this](gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg) { // error handler
                std::cout << "error_handler: " << fl << ": ec=" << ec << ", emsg=" << emsg << std::endl;
                if ( m_prof_timer->started() ) {
                    m_prof_timer->stop();
                }

                this->m_netio.get_worker().post([this, holder](){start(std::move(holder));});
            }
            ,[this](gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg, char *, std::size_t) { // read handler
                (void)holder;

                if ( ec ) {
                    std::cout << "read_handler: " << fl << ": ec=" << ec << ", emsg=" << emsg << std::endl;
                }
            }
            ,[this](gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg, const char *, std::size_t) { // write handler
                (void)holder;

                if ( ec ) {
                    std::cout << "write_handler: " << fl << ": ec=" << ec << ", emsg=" << emsg << std::endl;
                }
            }
        )
        ,m_ip{ip}
        ,m_port{port}
        ,m_prof_timer(gbp::net::create_one_shot_timer(w))
    {}

    void start(gbp::net::impl_holder holder = gbp::net::impl_holder()) {
        m_netio.async_connect(
             m_ip.c_str()
            ,m_port
            ,[this](gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg)
			 { on_connected(std::move(holder), fl, ec, emsg); }
            ,std::move(holder)
        );
    }
    void stop() {
        if ( !m_netio.get_socket()->is_open() )
            return;

        m_prof_timer->stop();

        m_netio.stop();
    }

private:
    void on_connected(gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg) {
        if ( ec ) {
            std::cout << "on_connected: " << fl << ": ec=" << ec << ", emsg=" << emsg << std::endl;

            return;
        }

        m_netio.add_service<gbp::net::ping_service>(
             1 // ping interval
            ,[](std::uint64_t time_us){ std::cout << "ping_time=" << time_us << "us" << std::endl; }
        );
        m_netio.add_service<gbp::net::remote_time_service>();
        m_netio.add_service<gbp::net::disconnect_service>();

        if ( m_netio.is_no_crypt() ) {
            start_timer(holder);
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

    void on_handshake(gbp::net::impl_holder holder, const char *fileline, int ec, const char *emsg) {
        if ( ec ) {
            std::cout << "handshake_handler(" << fileline << "): ec=" << ec << ", emsg=" << emsg << std::endl;
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
            s->disconnect(0, holder);
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

    gbp::net::netio m_netio;
    const std::string m_ip;
    const std::uint16_t m_port;
    gbp::net::timer_type m_prof_timer;
};

/*********************************************************************************************************************/

int main() {
    gbp::net::worker worker;

    client cli(worker, "127.0.0.1", 1234);
    cli.start();

    worker.run();

    return 0;
}

/*********************************************************************************************************************/

