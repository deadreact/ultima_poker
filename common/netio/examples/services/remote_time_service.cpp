
#include "remote_time_service.hpp"

#include <chrono>

namespace gbp {
namespace net {

/*********************************************************************************************************************/

struct remote_time_service::impl {
    impl(gbp::net::netio &netio)
        :m_netio{netio}
        ,m_started{false}
        ,m_reg{m_netio.create_registry("services.remote_time_service")}
        ,m_cb{}
    {
        m_reg->on("req_get_time", this, &impl::req_get_time);
        m_reg->on("rep_get_time", this, &impl::rep_get_time);
    }

    void get_time(gbp::net::impl_holder holder) {
        m_netio.call(std::move(holder), "services.remote_time_service", "req_get_time");
    }

    void req_get_time() {
        std::uint64_t time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        m_netio.call2("services.remote_time_service", "rep_get_time", time);
    }
    void rep_get_time(std::uint64_t time) {
        if ( m_cb ) {
            m_cb(time);
            m_cb = {};
        }
    }

    gbp::net::netio &m_netio;
    bool m_started;
    gbp::net::object_registry_ptr m_reg;
    std::function<void(std::uint64_t)> m_cb;
};

/*********************************************************************************************************************/

remote_time_service::remote_time_service(gbp::net::netio &netio)
    :gbp::net::service_base()
    ,pimpl(new impl(netio))
{}
remote_time_service::~remote_time_service()
{ delete pimpl; }

/*********************************************************************************************************************/

bool remote_time_service::start(gbp::net::impl_holder) {
    if ( pimpl->m_started )
        return false;

    pimpl->m_started = true;

    return pimpl->m_started;
}
void remote_time_service::stop() {
    pimpl->m_started = false;
}

bool remote_time_service::started() const { return pimpl->m_started; }

void remote_time_service::get_time(std::function<void(std::uint64_t)> cb, gbp::net::impl_holder holder) {
    pimpl->m_cb = std::move(cb);
    pimpl->get_time(std::move(holder));
}

/*********************************************************************************************************************/

} // ns net
} // ns gbp
