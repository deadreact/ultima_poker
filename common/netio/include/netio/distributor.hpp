
#ifndef __gbp__net__distributor_hpp
#define __gbp__net__distributor_hpp

#include <netio/object_registry.hpp>
#include <netio/profdata.hpp>

#include <string>
#include <functional>
#include <memory>

namespace gbp {
namespace net {

/***************************************************************************/

struct distributor {
    distributor(const distributor &) = delete;

    distributor &operator=(const distributor &) = delete;

    distributor();

    virtual ~distributor();

    /** get object registry for specified object */
    object_registry_ptr create_registry(const std::string &object);

    /** check if specified object name already registered */
    bool object_registered(const std::string &object) const;

    /** invokation */
    void invoke(const std::string &object, const std::string &proc, const char *body, std::size_t body_size);

    /** local invoke */
    template<typename... Args>
    void local_invoke(const std::string &object, const std::string &proc, const Args&... args) {
        const auto targs = std::make_tuple(std::cref(args)...);
        _local_invoke(object, proc, &targs);
    };

    void get_profdata(const get_profdata_cb &f) const;

private:
    void deregister(object_registry *reg);

    void _local_invoke(const std::string &object, const std::string &proc, const void *args);

    struct impl;
    std::unique_ptr<impl> m_pimpl;
};

/***************************************************************************/

} // ns net
} // ns gbp

#endif // __gbp__net__distributor_hpp
