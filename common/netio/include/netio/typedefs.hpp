
#ifndef __gbp__net__typedefs_hpp
#define __gbp__net__typedefs_hpp

#include <functional>
#include <memory>

#include <cstdint>

namespace gbp {
namespace net {

/**************************************************************************/

using impl_holder = std::shared_ptr<void>;

struct i_socket;
using socket_type = std::shared_ptr<i_socket>;

using on_error_handler     = std::function<void(impl_holder holder, const char *fl, int ec, const char *emsg)>;
using on_handshake_handler = std::function<void(impl_holder holder, const char *fl, int ec, const char *emsg)>;
using on_accept_handler    = std::function<void(impl_holder holder, const char *fl, int ec, const char *emsg, socket_type sock)>;
using on_connect_handler   = std::function<void(impl_holder holder, const char *fl, int ec, const char *emsg)>;
using on_read_handler      = std::function<void(impl_holder holder, const char *fl, int ec, const char *emsg, char *buf, std::size_t size)>;
using on_write_handler     = std::function<void(impl_holder holder, const char *fl, int ec, const char *emsg, const char *buf, std::size_t size)>;
using on_timeout_handler   = std::function<void(impl_holder holder, const char *fl, int ec, const char *emsg)>;
using on_incoming_handler  = std::function<void(impl_holder holder, const char *cls, const char *func, const char *buf, std::size_t size)>;
using on_netio_stop_handler= std::function<void(impl_holder holder)>; // called when netio::stop() called

/**************************************************************************/

} // ns net
} // ns gbp

#endif // __gbp__net__typedefs_hpp
