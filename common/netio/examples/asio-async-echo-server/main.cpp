
#include "../rpc/api1.hpp"

#include <netio/netio.hpp>

#include <boost/asio/io_service.hpp>

#include <iostream>
#include <boost/asio/ip/tcp.hpp>

/*********************************************************************************************************************/

// server -> client
struct reply: gbp::api1::reply {
    reply(gbp::net::netio &remote)
        :m_remote{remote}
    {}

    void recv_idx(std::uint32_t idx) override {
//        std::cout << "recv_idx=" << idx << std::endl;
        m_remote.call2("reply", "recv_idx", idx);
    }

private:
    gbp::net::netio &m_remote;
};

// client -> server
struct request: gbp::api1::request {
    request(gbp::net::netio &remote, reply &rep)
        :m_remote{remote}
        ,reg{remote.create_registry("request")}
        ,m_rep{rep}
    {
        reg->on("send_idx", this, &request::send_idx);
    }

    void send_idx(std::uint32_t idx) override {
//        std::cout << "send_idx=" << idx << std::endl;
        m_rep.recv_idx(idx+1);
    }

private:
    gbp::net::netio &m_remote;
    gbp::net::object_registry_ptr reg;
    reply &m_rep;
};

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

                m_timer->stop();
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
        ,m_timer{gbp::net::create_one_shot_timer(m_netio.get_worker())}
        ,m_rep(m_netio)
        ,m_req(m_netio, m_rep)
    {
        std::cout << "session ctor:" << this << std::endl;
    }

    virtual ~session()
    {
        std::cout << "session dtor:" << this << std::endl;
    }

    void start(gbp::net::impl_holder holder) {
        start_timer(holder);

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
        m_netio.start(std::move(holder));
    }

    void start_timer(gbp::net::impl_holder holder) {
        m_timer->start(
             5000
            ,[this](gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg)
             { on_timeout(std::move(holder), fl, ec, emsg); }
            ,std::move(holder)
        );
    }
    void on_timeout(gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg) {
        (void)fl;
        (void)emsg;

        if ( ec == gbp::net::error::operation_aborted ) return;

        auto cb = [](const std::string &object, const std::string &proc, const gbp::net::profdata &data) {
            std::cout << object << "." << proc << ": " << data.dump() << std::endl;
        };

        m_netio.get_profdata(cb);

        start_timer(std::move(holder));
    }

private:
    gbp::net::netio m_netio;
    gbp::net::timer_type m_timer;
    reply m_rep;
    request m_req;
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
