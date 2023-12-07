
#ifndef __gbp__net__client_skeleton_hpp
#define __gbp__net__client_skeleton_hpp

#include <netio/netio.hpp>

namespace boost {
namespace asio {
class io_service;
} // ns asio
} // ns boost

namespace gbp {
namespace net {

/*************************************************************************************************/

struct client {
    client() = delete;
    client(const client &) = delete;
    client& operator=(const client &) = delete;
    client& operator=(client &&) = delete;

    client(
         worker &w
        ,bool no_crypt_proto = false
        ,std::size_t inactivity_timeout = gbp::net::netio::k_inactivity_timeout
        ,std::size_t max_body_size = gbp::net::netio::k_max_body_size
        ,std::size_t compress_if_greater = gbp::net::netio::k_compress_if_greater
        ,on_error_handler eh = on_error_handler()
        ,on_read_handler rh = on_read_handler()
        ,on_write_handler wh = on_write_handler()
        ,on_incoming_handler ih = on_incoming_handler()
        ,on_netio_stop_handler sh = on_netio_stop_handler()
    );
    client(client &&);
    virtual ~client();

    // synchronous start
    std::pair<
         int         // ec
        ,const char* // emsg
    >
    start(const char *ip, std::uint16_t port);

    void async_start(
         const char *ip
        ,std::uint16_t port
        ,on_connect_handler cb
        ,impl_holder holder = impl_holder()
    );

    std::size_t get_inactivity_timeout() const;
    void set_inactivity_timeout(std::size_t seconds);

    std::size_t get_max_body_size() const;
    void set_max_body_size(std::size_t size);

    std::size_t get_compress_if_greater() const;
    void set_compress_if_greater(std::size_t size);

    const on_error_handler& get_error_handler() const;
    void set_error_handler(on_error_handler cb);

    const on_read_handler& get_read_handler() const;
    void set_read_handler(on_read_handler cb);

    const on_write_handler& get_write_handler() const;
    void set_write_handler(on_write_handler cb);

    const on_incoming_handler& get_incoming_handler() const;
    void set_incoming_handler(on_incoming_handler cb);

    const on_netio_stop_handler& get_netio_stop_handler() const;
    void set_netio_stop_handler(on_netio_stop_handler cb);

    bool started() const;
    void stop();

    netio& get_netio();
    const netio& get_netio() const;

    object_registry_ptr create_registry(const std::string &object);

    template<typename ConstCharPtr, typename... Args>
    typename std::enable_if<
        std::is_same<ConstCharPtr, const char *>::value
    >::type
    call(impl_holder holder, ConstCharPtr api, Args&& ...args) {
        get_netio().call(std::move(holder), api, std::forward<Args>(args)...);
    }
    template<std::size_t N, typename ...Args>
    void call(impl_holder holder, const char (&api)[N], Args&& ...args) {
        get_netio().call<N>(std::move(holder), api, std::forward<Args>(args)...);
    }
    template<std::size_t ON, std::size_t FN, typename ...Args>
    void call(impl_holder holder, const char (&obj)[ON], const char (&func)[FN], Args&& ...args) {
        get_netio().call<ON, FN>(std::move(holder), obj, func, std::forward<Args>(args)...);
    }

    template<typename ConstCharPtr, typename... Args>
    typename std::enable_if<
        std::is_same<ConstCharPtr, const char *>::value
    >::type
    call2(ConstCharPtr api, Args&& ...args) {
        get_netio().call2(api, std::forward<Args>(args)...);
    }
    template<std::size_t N, typename ...Args>
    void call2(const char (&api)[N], Args&& ...args) {
        get_netio().call2<N>(api, std::forward<Args>(args)...);
    }
    template<std::size_t ON, std::size_t FN, typename ...Args>
    void call2(const char (&obj)[ON], const char (&func)[FN], Args&& ...args) {
        get_netio().call2<ON, FN>(obj, func, std::forward<Args>(args)...);
    }

private:
    struct impl;
    impl *pimpl;
};

/*************************************************************************************************/

} // ns net
} // ns gbp

#endif // __gbp__net__client_skeleton_hpp
