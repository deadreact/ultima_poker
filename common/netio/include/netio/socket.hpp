
#ifndef __gbp__net__socket_hpp
#define __gbp__net__socket_hpp

#include <netio/typedefs.hpp>

#include <string>
#include <memory>

namespace gbp {
namespace net {

struct worker;

/**************************************************************************/

struct i_socket {
    virtual ~i_socket() {}

    virtual worker& get_worker() = 0;
    virtual void* get_handle() = 0;

    virtual bool is_open() const = 0;
    virtual int close() = 0; // returns 0 if closed, error_code instead

    virtual std::size_t bytes_avail() const = 0;

    virtual void connect(const char *ip, std::uint16_t port, int *ec, std::string *emsg) = 0;
    virtual void async_connect(const char *ip, std::uint16_t port, on_connect_handler cb, impl_holder holder = impl_holder()) = 0;

    virtual std::size_t read(char *ptr, std::size_t size, int *ec, std::string *emsg) = 0;
    virtual std::size_t read_some(char *ptr, std::size_t size, int *ec, std::string *emsg) = 0;
    virtual void async_read(char *buf, std::size_t size, on_read_handler cb, impl_holder holder = impl_holder()) = 0;
    virtual void async_read_some(char *buf, std::size_t size, on_read_handler cb, impl_holder holder = impl_holder()) = 0;

    virtual std::size_t write(const char *ptr, std::size_t size, int *ec, std::string *emsg) = 0;
    virtual std::size_t write_some(const char *ptr, std::size_t size, int *ec, std::string *emsg) = 0;
    virtual void async_write(const char *buf, std::size_t size, on_write_handler cb, impl_holder holder = impl_holder()) = 0;
    virtual void async_write_some(const char *buf, std::size_t size, on_write_handler cb, impl_holder holder = impl_holder()) = 0;
};

using socket_type = std::shared_ptr<i_socket>;

socket_type create_socket(worker &w);

/**************************************************************************/

} // ns net
} // ns gbp

#endif // __gbp__net__socket_hpp
