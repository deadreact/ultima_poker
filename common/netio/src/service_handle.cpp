
#include <netio/service_handle.hpp>
#include <netio/service_base.hpp>
#include <netio/typedefs.hpp>

namespace gbp {
namespace net {

/**************************************************************************/

struct service_handle::impl {
    impl(std::weak_ptr<service_base> b)
        :m_handle{std::move(b)}
    {}

    std::weak_ptr<service_base> m_handle;
};

/**************************************************************************/

service_handle::service_handle(std::weak_ptr<service_base> b)
    :pimpl(new impl(std::move(b)))
{}

service_handle::service_handle(const service_handle &r)
    :pimpl(new impl(r.pimpl->m_handle))
{}

service_handle& service_handle::operator= (const service_handle &r) {
    pimpl->m_handle = r.pimpl->m_handle;

    return *this;
}

service_handle::service_handle(service_handle &&r)
    :pimpl{r.pimpl}
{ r.pimpl = nullptr; }

service_handle& service_handle::operator= (service_handle &&r) {
    delete pimpl;
    pimpl = r.pimpl;
    r.pimpl  = nullptr;

    return *this;
}

service_handle::~service_handle()
{ delete pimpl; }

/**************************************************************************/

std::weak_ptr<service_base> service_handle::get_ptr() { return pimpl->m_handle; }
const std::weak_ptr<service_base> service_handle::get_ptr() const { return pimpl->m_handle; }

service_handle::operator bool() const { return !pimpl->m_handle.expired(); }

/**************************************************************************/

bool service_handle::start(impl_holder holder) {
    if ( auto p = get_ptr().lock() ) {
        return p->start(std::move(holder));
    }

    return false;
}

void service_handle::stop() {
    if ( auto p = get_ptr().lock() ) {
        p->stop();
    }
}

bool service_handle::started() const {
    if ( auto p = get_ptr().lock() ) {
        return p->started();
    }

    return false;
}

/**************************************************************************/

} // ns net
} // ns gbp
