
#include "client.hpp"

#include <netio/netio.hpp>
#include <netio/ppdefs.hpp>

namespace gbp {
namespace net {

/*************************************************************************************************/

struct client::impl {
    impl(
         worker &w
        ,bool no_crypt_proto
        ,std::size_t inactivity_timeout
        ,std::size_t max_body_size
        ,std::size_t compress_if_greater
        ,on_error_handler eh
        ,on_read_handler rh
        ,on_write_handler wh
        ,on_incoming_handler ih
        ,on_netio_stop_handler sh
    )
        :m_worker(w)
        ,m_netio(
             m_worker
            ,(!no_crypt_proto ? netio::client : netio::client|netio::no_crypt)
            ,inactivity_timeout
            ,max_body_size
            ,compress_if_greater
            ,std::move(eh)
            ,std::move(rh)
            ,std::move(wh)
            ,std::move(ih)
            ,std::move(sh)
        )
    {}

    std::pair<int, const char*>
    start(const char *ip, std::uint16_t port) {
        if ( started() )
            return {-1, "Client already started"};

        int ec{};
        std::string emsg;

        m_netio.connect(ip, port, &ec, &emsg);
        if ( ec ) {
            return {ec, emsg.c_str()};
        }

        if ( m_netio.is_no_crypt() ) {
            m_netio.start();
        } else {
            m_netio.handshake(&ec, &emsg);
            if (ec) {
                return {ec, emsg.c_str()};
            }
            m_netio.start();
        }

        return {0, nullptr};
    }

    void async_start(const char *ip, std::uint16_t port, on_connect_handler cb, impl_holder holder) {
        if ( started() ) {
            cb(std::move(holder), __NETIO_MAKE_FILELINE(), -1, "Already started");

            return;
        }

        m_ip = ip;
        m_port = port;

        m_netio.async_connect(
             m_ip.c_str()
            ,m_port
            ,[this, cb]
             (impl_holder holder, const char *fl, int ec, const char *emsg){
                on_async_connect(fl, ec, emsg, std::move(cb), std::move(holder));
            }
            ,std::move(holder)
        );
    }
    void on_async_connect(const char *fl, int ec, const char *emsg, on_connect_handler cb, impl_holder holder) {
        (void)fl;

        if ( ec ) {
            if ( cb ) cb(std::move(holder), __NETIO_MAKE_FILELINE(), ec, emsg);

            return;
        }

        if ( m_netio.is_no_crypt() ) {
            cb(holder, __NETIO_MAKE_FILELINE(), 0, nullptr);

            m_netio.start(std::move(holder));
        } else {
            m_netio.async_handshake(
                [this, cb]
                (impl_holder holder, const char *fl, int ec, const char *emsg)
                { on_async_handshake(fl, ec, emsg, std::move(cb), std::move(holder)); }
                ,std::move(holder)
            );
        }
    }
    void on_async_handshake(const char *fl, int ec, const char *emsg, on_connect_handler cb, impl_holder holder) {
        (void)fl;

        if ( ec ) {
            if ( cb ) cb(std::move(holder), __NETIO_MAKE_FILELINE(), ec, emsg);

            return;
        }

        cb(holder, __NETIO_MAKE_FILELINE(), 0, nullptr);

        m_netio.start(std::move(holder));
    }

    bool started() const { return m_netio.started(); }

    void stop() { m_netio.stop(); }

    worker &m_worker;
    std::string m_ip;
    std::uint16_t m_port;
    netio m_netio;
};

/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/

client::client(
     worker &w
    ,bool no_crypt_proto
    ,std::size_t inactivity_timeout
    ,std::size_t max_body_size
    ,std::size_t compress_if_greater
    ,on_error_handler eh
    ,on_read_handler rh
    ,on_write_handler wh
    ,on_incoming_handler ih
    ,on_netio_stop_handler sh
)
    :pimpl(new impl(
         w
        ,no_crypt_proto
        ,inactivity_timeout
        ,max_body_size
        ,compress_if_greater
        ,std::move(eh)
        ,std::move(rh)
        ,std::move(wh)
        ,std::move(ih)
        ,std::move(sh)
    ))
{}

client::client(client &&r)
    :pimpl(r.pimpl)
{
    r.pimpl = nullptr;
}

client::~client()
{ delete pimpl; }

/*************************************************************************************************/

std::pair<int, const char *>
client::start(const char *ip, std::uint16_t port) { return pimpl->start(ip, port); }

void client::async_start(
     const char *ip
    ,std::uint16_t port
    ,on_connect_handler cb
    ,impl_holder holder)
{
    pimpl->async_start(ip, port, std::move(cb), std::move(holder));
}

/*************************************************************************************************/

std::size_t client::get_inactivity_timeout() const { return pimpl->m_netio.get_inactivity_timeout(); }
void client::set_inactivity_timeout(std::size_t seconds) { pimpl->m_netio.set_inactivity_timeout(seconds); }

std::size_t client::get_max_body_size() const { return pimpl->m_netio.get_max_body_size(); }
void client::set_max_body_size(std::size_t size) { pimpl->m_netio.set_max_body_size(size); }

std::size_t client::get_compress_if_greater() const { return pimpl->m_netio.get_compress_if_greater(); }
void client::set_compress_if_greater(std::size_t size) { pimpl->m_netio.set_compress_if_greater(size); }

const on_error_handler& client::get_error_handler() const { return pimpl->m_netio.get_error_handler(); }
void client::set_error_handler(on_error_handler cb) { pimpl->m_netio.set_error_handler(std::move(cb)); }

const on_read_handler& client::get_read_handler() const { return pimpl->m_netio.get_read_handler(); }
void client::set_read_handler(on_read_handler cb) { pimpl->m_netio.set_read_handler(std::move(cb)); }

const on_write_handler& client::get_write_handler() const { return pimpl->m_netio.get_write_handler(); }
void client::set_write_handler(on_write_handler cb) { pimpl->m_netio.set_write_handler(std::move(cb)); }

const on_incoming_handler& client::get_incoming_handler() const { return pimpl->m_netio.get_incoming_handler(); }
void client::set_incoming_handler(on_incoming_handler cb) { pimpl->m_netio.set_incoming_handler(std::move(cb)); }

const on_netio_stop_handler& client::get_netio_stop_handler() const { return pimpl->m_netio.get_netio_stop_handler(); }
void client::set_netio_stop_handler(on_netio_stop_handler cb) { pimpl->m_netio.set_netio_stop_handler(std::move(cb)); }

/*************************************************************************************************/

bool client::started() const { return pimpl->started(); }

void client::stop() { pimpl->stop(); }

/*************************************************************************************************/

netio& client::get_netio() { return pimpl->m_netio; }
const netio& client::get_netio() const { return pimpl->m_netio; }

object_registry_ptr client::create_registry(const std::string &object) {
    return pimpl->m_netio.create_registry(object);
}

/*************************************************************************************************/

} // ns net
} // ns gbp
