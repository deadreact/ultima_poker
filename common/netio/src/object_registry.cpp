
#include <netio/object_registry.hpp>
#include <netio/invokers.hpp>

#include <map>

namespace gbp {
namespace net {

/***************************************************************************/

struct object_registry::impl {
    impl(const std::string& object)
        :m_object(object)
        ,m_map()
    {}

    std::string m_object;
    std::map<std::string, detail::i_invoker_ptr> m_map;
};

/***************************************************************************/

object_registry::object_registry(const std::string &object)
    :m_pimpl(new impl(object))
{}

object_registry::~object_registry()
{}

const std::string& object_registry::object() const { return m_pimpl->m_object; }

/***************************************************************************/

bool object_registry::registered(const std::string &mf) const {
    return m_pimpl->m_map.find(mf) != m_pimpl->m_map.end();
}

detail::i_invoker_ptr object_registry::get_invoker(const std::string &mf) const {
    auto it = m_pimpl->m_map.find(mf);
    if ( it != m_pimpl->m_map.end() ) {
        return it->second;
    } else {
        return detail::i_invoker_ptr();
    }
}

/***************************************************************************/

void object_registry::get_profdata(const get_profdata_cb &f) const {
    for ( const auto &it: m_pimpl->m_map ) {
        f(it.first, it.second->get_profdata());
    }
}

/***************************************************************************/

void object_registry::insert(std::string mf, detail::i_invoker_ptr ptr) {
    if ( !m_pimpl->m_map.emplace(std::move(mf), std::move(ptr)).second ) {
        const std::string msg = "Can't insert invoker for \"" + mf + "\"";
        throw std::runtime_error(msg);
    }
}

/***************************************************************************/

} // ns net
} // ns gbp
