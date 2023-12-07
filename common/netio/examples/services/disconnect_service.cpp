
#include "disconnect_service.hpp"

namespace gbp {
namespace net {

/*********************************************************************************************************************/

struct disconnect_service::impl {
    impl(gbp::net::netio &netio)
        :m_netio{netio}
        ,m_started{false}
        ,m_reg{m_netio.create_registry("services.disconnect_service")}
        ,m_cb{}
    {
        m_reg->on("on_disconnect", this, &impl::on_disconnect);
    }

    void on_disconnect(std::int32_t ec) {
        if ( m_cb ) {
            m_cb(ec);
        }
    }

    void disconnect(std::int32_t ec, impl_holder holder) {
        m_netio.call(std::move(holder), "services.disconnect_service", "on_disconnect", ec);
    }

    gbp::net::netio &m_netio;
    bool m_started;
    gbp::net::object_registry_ptr m_reg;
    std::function<void(std::int32_t ec)> m_cb;
};

/*********************************************************************************************************************/

disconnect_service::disconnect_service(gbp::net::netio &netio)
    :gbp::net::service_base()
    ,pimpl(new impl(netio))
{}
disconnect_service::~disconnect_service()
{ delete pimpl; }

/*********************************************************************************************************************/

bool disconnect_service::start(gbp::net::impl_holder) {
    if ( pimpl->m_started )
        return false;

    pimpl->m_started = true;

    return pimpl->m_started;
}
void disconnect_service::stop() { pimpl->m_started = false; }

bool disconnect_service::started() const { return pimpl->m_started; }

void disconnect_service::set_disconnect_handler(std::function<void(std::int32_t ec)> cb) { pimpl->m_cb = std::move(cb); }

void disconnect_service::disconnect(std::int32_t ec, impl_holder holder) { pimpl->disconnect(ec, std::move(holder)); }

/*********************************************************************************************************************/

} // ns net
} // ns gbp
