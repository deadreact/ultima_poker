
#include "../rpc/api1.hpp"

#include <netio/netio.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <iostream>

/*********************************************************************************************************************/

// client -> server
struct request: gbp::api1::request {
    request(gbp::net::netio &remote)
        :m_remote{remote}
    {}

    void send_idx(std::uint32_t idx) override {
//        m_remote.call2("request.send_idx", idx);
        auto buf = m_remote.prepare_packet("request", "send_idx", idx);
        m_remote.send_packet2(std::move(buf));
    }

private:
    gbp::net::netio &m_remote;
};

// server -> client
struct reply: gbp::api1::reply {
    reply(gbp::net::netio &remote, request &req)
        :m_remote{remote}
        ,m_reg{remote.create_registry("reply")}
        ,m_req{req}
    {
        m_reg->on("recv_idx", this, &reply::recv_idx);
    }

    void recv_idx(std::uint32_t idx) override {
        if ( (idx % 10000) == 0 ) {
            std::cout << "recv_idx=" << idx << std::endl;
        }

        m_req.send_idx(idx);
//        std::size_t t = (idx==4 ? 10*1000 : 9*1000);
//        gbp::net::timer_type timer = gbp::net::create_one_shot_timer(m_remote.get_worker());
//        timer->start(
//             t
//            ,[this, idx, timer]
//             (gbp::net::impl_holder, const char *fl, int ec, const char *emsg) {
//                if ( ec ) {
//                    std::cout << "timer_handler: fl=" << fl << ", ec=" << ec << ", emsg=" << emsg << std::endl;
//                    return;
//                }
//                if ( !m_remote.started() ) {
//                    std::cout << "timer_handler: m_remote.started() == false" << std::endl;
//                    return;
//                }
//
//                m_req.send_idx(idx);
//             }
//        );
    }

private:
    gbp::net::netio &m_remote;
    gbp::net::object_registry_ptr m_reg;
    request m_req;
};

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
//                if ( ec == gbp::net::error::timed_out )

                this->m_netio.get_worker().post([this, holder](){start(std::move(holder));});
//                start(std::move(holder));
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
        ,m_req(m_netio)
        ,m_rep(m_netio, m_req)
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

        if ( m_netio.is_no_crypt() ) {
            start_timer(holder);
            m_netio.start(std::move(holder));
            m_req.send_idx(0);
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

        m_netio.start(std::move(holder));

        m_req.send_idx(0);
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
        if ( ec == boost::asio::error::operation_aborted )
            return;

        if ( ec ) {
            std::cout << "on_timeout: " << fl << ": ec=" << ec << ", emsg=" << emsg << std::endl;

            return;
        }

        auto cb = [](const std::string &object, const std::string &proc, const gbp::net::profdata &data) {
            std::cout << object << "." << proc << ": " << data.dump() << std::endl;
        };

        m_netio.get_profdata(cb);

        start_timer(std::move(holder));
    }

    gbp::net::netio m_netio;
    const std::string m_ip;
    const std::uint16_t m_port;
    gbp::net::timer_type m_prof_timer;
    request m_req;
    reply m_rep;
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
