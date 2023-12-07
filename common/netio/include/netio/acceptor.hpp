
#ifndef __gbp__net__acceptor_hpp
#define __gbp__net__acceptor_hpp

#include <netio/worker.hpp>
#include <netio/socket.hpp>
#include <netio/typedefs.hpp>

#include <memory>
#include <string>

namespace gbp {
namespace net {

struct worker;

/**************************************************************************/

struct i_acceptor {
    virtual ~i_acceptor() {}

    virtual worker& get_worker() const = 0;
    virtual socket_type accept(int *ec, std::string *emsg) = 0;
    virtual void async_accept(socket_type sock, on_accept_handler cb, impl_holder holder = impl_holder()) = 0;
    virtual void async_accept(on_accept_handler cb, impl_holder holder = impl_holder()) = 0;

    virtual void close() = 0;
};

using acceptor_type = std::shared_ptr<i_acceptor>;

acceptor_type create_acceptor(worker &w, const char *ip, std::uint16_t port);

/**************************************************************************/

} // ns net
} // ns gbp

#endif // __gbp__net__acceptor_hpp
