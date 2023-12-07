
#include <netio/error.hpp>
#include <netio/timer.hpp>

#include "client.hpp"

#include "api.hpp"

#include <boost/asio/io_service.hpp>

#include <iostream>

/*************************************************************************************************/

struct requests: api::myrequest {
    requests(gbp::net::client &client)
        :client{client}
    {}

    void req0()      override { client.call2("req.stat.games_statistic"); }
    void req1(int v) override { client.call2("requests.req1", v); }

private:
    gbp::net::client &client;
};

struct replies: api::myreplies {
    replies(gbp::net::worker &w, gbp::net::client &client, requests &reqs)
        :m_worker{w}
        ,m_client{client}
        ,m_reqs{reqs}
        ,m_reg{client.create_registry("rep.stat")}
    {
        m_reg->on("games_statistic", this, &replies::rep0);
        m_reg->on("rep1", this, &replies::rep1);
    }

    void rep0(int v0, int v1) override {
        std::cout << "replies::rep0(v0=" << v0 << ", v1=" << v1 << ")" << std::endl;

        gbp::net::timer_type timer = gbp::net::create_one_shot_timer(m_worker);
        timer->start(
             1000
            ,[this, timer](gbp::net::impl_holder, const char *, int ec, const char *) {
                if ( ec ) return;
                if ( !m_client.started() ) {
                    std::cout << "client is not open!" << std::endl;
                    return;
                }
                m_reqs.req0();
            }
        );

//        m_reqs.req0();
//        m_client.stop();
    }
    void rep1(int v) override {
        std::cout << "replies::rep1(" << v << ")" << std::endl;
    }

    gbp::net::worker &m_worker;
    gbp::net::client &m_client;
    requests &m_reqs;
    gbp::net::object_registry_ptr m_reg;
};

/*************************************************************************************************/

struct my_client {
    my_client() = delete;
    my_client(const my_client &r) = delete;
    my_client& operator= (const my_client &r) = delete;

    my_client(gbp::net::worker &w, const char *server_ip, std::uint16_t server_port)
        :m_worker{w}
        ,m_client{
             m_worker
            ,true // true - no_crypt
            ,gbp::net::netio::k_inactivity_timeout
            ,gbp::net::netio::k_max_body_size
            ,gbp::net::netio::k_compress_if_greater
            ,[this](gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg)
             {error_handler(std::move(holder), fl, ec, emsg);}
        }
        ,m_server_ip{server_ip}
        ,m_server_port{server_port}
        ,m_reqs{m_client}
        ,m_reps{m_worker, m_client, m_reqs}
    {
        m_client.get_netio().set_incoming_handler(
            [this](gbp::net::impl_holder /*holder*/, const char *cls, const char *func, const char */*buf*/, std::size_t size) {
                std::cout << "this=" << this << ": " << cls << "." << func << ": body_size=" << size << std::endl;
            }
        );
    }

    ~my_client()
    {}

    void start(gbp::net::impl_holder holder = gbp::net::impl_holder()) {
        m_client.async_start(
             m_server_ip.c_str()
            ,m_server_port
            ,[this](gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg) {
                start_handler(std::move(holder), fl, ec, emsg);
            }
            ,std::move(holder)
        );
    }

private:
    void error_handler(gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg) {
        std::cerr << "error_handler(): fl=" << fl << ", ec=" << ec << ", emsg=" << emsg << std::endl;

        m_worker.post(
            [this, holder]()
            { start(std::move(holder)); }
        );
    }

    void start_handler(gbp::net::impl_holder /*holder*/, const char *fl, int ec, const char *emsg) {
        if ( ec ) {
            std::cout << "start_handler(): fl=" << fl << ", ec=" << ec << ", emsg=" << emsg << std::endl;

            return;
        }

        m_reqs.req0();
    }

    gbp::net::worker &m_worker;
    gbp::net::client m_client;
    std::string m_server_ip;
    std::uint16_t m_server_port;
    requests m_reqs;
    replies m_reps;
};

/*************************************************************************************************/

int main(int, char **) {
    static const char *server_ip = "127.0.0.1";
    static const std::uint16_t server_port = 44550;

    gbp::net::worker worker;

    my_client myc(worker, server_ip, server_port);
    myc.start();

    worker.run();

    return EXIT_SUCCESS;
}

/*************************************************************************************************/
