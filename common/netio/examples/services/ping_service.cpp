
#include "ping_service.hpp"

#include <iostream>

namespace gbp {
namespace net {

/*********************************************************************************************************************/
/*
 * client    |   server               |  client
  req_ping  ->  req_ping -> rep_ping -> rep_ping
 */

struct ping_service::impl {
    impl(gbp::net::netio &netio, std::size_t interval, std::function<void(std::uint64_t time_us)> cb)
        :m_netio{netio}
        ,m_interval{interval}
        ,m_cb{std::move(cb)}
        ,m_started{false}
        ,m_ping_timer{gbp::net::create_one_shot_timer(m_netio.get_worker())}
        ,m_reg{m_netio.create_registry("services.ping_service")}
    {
        m_reg->on("req_ping", this, &impl::req_ping);
        m_reg->on("rep_ping", this, &impl::rep_ping);
    }

    bool start(gbp::net::impl_holder holder) {
        if ( m_started )
            return false;
        m_started = true;

        start_timer(std::move(holder));

        return true;
    }
    void stop() {
        m_ping_timer->stop();
        m_started = false;
    }
    bool started() const { return m_started; }

    void start_timer(gbp::net::impl_holder holder) {
        m_ping_timer->start(
            m_interval*1000
            ,[this]
             (gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg)
             { on_timer_timeout(std::move(holder), fl, ec, emsg); }
            ,std::move(holder)
        );
    }
    void on_timer_timeout(gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg) {
        (void)fl;
        (void)emsg;
        if ( ec ) {
//            std::cout << "ping_service::on_timer_timeout: fl=" << fl << ", ec=" << ec << ", emsg=" << emsg << std::endl;
            return;
        }

        if ( m_netio.started() ) {
            std::uint64_t time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            ping(holder, time);
            start_timer(holder);
        }
    }

    /*  */
    void ping(gbp::net::impl_holder holder, std::uint64_t time) {
        m_netio.call(std::move(holder), "services.ping_service", "req_ping", time);
    }
    void req_ping(std::uint64_t time) {
//        std::cout << "ping_service::req_ping()" << std::endl;
        m_netio.call2("services.ping_service", "rep_ping", time);
    }
    void rep_ping(std::uint64_t time) {
        std::uint64_t local_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        std::uint64_t diff = (local_time - time)/2;
//        std::cout << "ping_service: time=" << diff << "us" << std::endl;
        if ( m_cb ) {
            m_cb(diff);
        }
    }

    gbp::net::netio &m_netio;
    std::size_t m_interval;
    std::function<void(std::uint64_t time_us)> m_cb;
    bool m_started;

    gbp::net::timer_type m_ping_timer;
    gbp::net::object_registry_ptr m_reg;
};

/*********************************************************************************************************************/

ping_service::ping_service(gbp::net::netio &netio, std::size_t interval, std::function<void(std::uint64_t time_us)> cb)
    :gbp::net::service_base()
    ,pimpl(new impl(netio, interval, std::move(cb)))
{}

ping_service::~ping_service()
{ delete pimpl; }

/*********************************************************************************************************************/

bool ping_service::start(gbp::net::impl_holder holder) {
    return pimpl->start(std::move(holder));
}
void ping_service::stop() { pimpl->stop(); }

bool ping_service::started() const { return pimpl->started(); }

/*********************************************************************************************************************/

} // ns net
} // ns gbp
