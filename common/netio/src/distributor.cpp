
#include <cassert>
#include <string>
#include <map>
#include <memory>

#include <netio/distributor.hpp>
#include <netio/object_registry.hpp>

#define ___MYSTRINGIZE(x) #x
#define __MYSTRINGIZE(x) ___MYSTRINGIZE(x)

namespace gbp {
namespace net {

/***************************************************************************/

struct distributor::impl {
    impl()
    {}

    detail::i_invoker_ptr get_invoker(const std::string &object, const std::string &proc) {
        const auto reg_it = m_map.find(object);
        if ( reg_it == m_map.end() ) {
            const std::string msg = __FILE__ "(" __MYSTRINGIZE(__LINE__) "): registry for object \"" + object + "\" is not registered";
            throw std::runtime_error(msg);
        }

        if ( object_registry_ptr reg = reg_it->second.lock() ) {
            if ( auto invoker = reg->get_invoker(proc) ) {
                return invoker;
            } else {
                const std::string msg = __FILE__ "(" __MYSTRINGIZE(__LINE__) "): registry for proc \"" + proc + "\" is not registered";
                throw std::runtime_error(msg);
            }
        } else {
            m_map.erase(reg_it);
        }

        return detail::i_invoker_ptr();
    }

    using registry_map = std::map<std::string,std::weak_ptr<object_registry>>;
    registry_map m_map;
};

/***************************************************************************/

distributor::distributor()
    :m_pimpl(new impl)
{}

distributor::~distributor()
{}

/***************************************************************************/

object_registry_ptr distributor::create_registry(const std::string &object) {
    if ( m_pimpl->m_map.find(object) != m_pimpl->m_map.end() ) {
        const std::string msg = __FILE__ "(" __MYSTRINGIZE(__LINE__) "): registry for object \"" + object + "\" is already registered";
        throw std::runtime_error(msg);
    }

    object_registry_ptr reg(new object_registry(object), [this](object_registry *p) { deregister(p); });
    m_pimpl->m_map.insert(std::make_pair(object, reg));

    return reg;
}

/***************************************************************************/

bool distributor::object_registered(const std::string &object) const {
    return m_pimpl->m_map.find(object) != m_pimpl->m_map.end();
}

/***************************************************************************/

#ifndef __NETIO_TRACEPOINT1
#   define __NETIO_TRACEPOINT1
#endif // __NETIO_TRACEPOINT1

#ifndef __NETIO_TRACEPOINT2
#   define __NETIO_TRACEPOINT2
#endif // __NETIO_TRACEPOINT2

void distributor::invoke(
     const std::string &object
    ,const std::string &proc
    ,const char *body
    ,std::size_t body_size)
{
    auto invoker = m_pimpl->get_invoker(object, proc);
    auto beg = gbp::net::profdata::clock_type::now();

    __NETIO_TRACEPOINT1
    invoker->invoke(body, body_size);
    __NETIO_TRACEPOINT2

    auto end = gbp::net::profdata::clock_type::now();
    invoker->update_profdata(body_size, end-beg);
}

void distributor::_local_invoke(
     const std::string &object
    ,const std::string &proc
    ,const void *args)
{
    auto invoker = m_pimpl->get_invoker(object, proc);
    invoker->local_invoke(args);
}

/***************************************************************************/

void distributor::get_profdata(const get_profdata_cb &f) const {
    for ( const auto &reg_it: m_pimpl->m_map ) {
        if ( object_registry_ptr reg = reg_it.second.lock() ) {
            const std::string &object = reg_it.first;
            auto object_reg = [&f, &object](const std::string &proc, const profdata &data){
                f(object, proc, data);
            };
            reg->get_profdata(object_reg);
        }
    }
}

/***************************************************************************/

void distributor::deregister(object_registry *reg) {
    assert(reg != nullptr && "null pointer error");
    m_pimpl->m_map.erase(reg->object());
    delete reg;
}

/***************************************************************************/

#undef __MYSTRINGIZE
#undef ___MYSTRINGIZE

} // ns net
} // ns gbp
