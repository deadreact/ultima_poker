
#ifndef __gbo__net__service_base_hpp
#define __gbo__net__service_base_hpp

#include <netio/typedefs.hpp>
#include <netio/service_handle.hpp>

namespace gbp {
namespace net {

struct netio;

/**************************************************************************/

struct service_base {
    virtual ~service_base() {}

    virtual bool start(impl_holder holder = impl_holder()) = 0;
    virtual void stop() = 0;

    virtual bool started() const = 0;
};

/**************************************************************************/

} // ns net
} // ns gbp

#endif // __gbo__net__service_base_hpp
