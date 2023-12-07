
#include <netio/ppdefs.hpp>
#include <netio/timer.hpp>
#include <netio/netio.hpp>
#include <netio/handler_allocator.hpp>
#include <netio/preallocated_handler_invoker.hpp>
#include <netio/make_preallocated_handler.hpp>

#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>

namespace gbp {
namespace net {

/**************************************************************************/

struct asio_socket: i_socket {
    asio_socket(worker &w)
        :i_socket{}
        ,m_worker{w}
        ,m_read_allocator{}
        ,m_write_allocator{}
        ,m_sock{*static_cast<boost::asio::io_service *>(m_worker.get_handle())}
    {}

    virtual ~asio_socket() {
        close();
    }

    virtual worker& get_worker() override { return m_worker; }
    virtual void* get_handle() override { return &m_sock; }

    virtual void connect(const char *ip, std::uint16_t port, int *ec, std::string *emsg) override {
        boost::system::error_code bec;
        const boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(ip), port);
        m_sock.connect(ep, bec);

        if (ec) *ec = bec.value();
        if (emsg) *emsg = bec.message();
    }
    virtual void async_connect(const char *ip, std::uint16_t port, on_connect_handler cb, impl_holder holder) override {
        const boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(ip), port);
        m_sock.async_connect(
             ep
            ,[this, cb, holder]
             (const boost::system::error_code &ec)
             { cb(std::move(holder), __NETIO_MAKE_FILELINE(), ec.value(), ec.message().c_str()); }
        );
    }

    virtual bool is_open() const override { return m_sock.is_open(); }

    virtual std::size_t bytes_avail() const override {
        if ( m_sock.is_open() ) {
            boost::system::error_code ec;
            return m_sock.available(ec);
        }

        return 0;
    }

    virtual int close() override {
        boost::system::error_code ec;

        m_sock.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
        if ( ec ) {
            return ec.value();
        }

        m_sock.close(ec);
        if ( ec ) {
            return ec.value();
        }

        return 0;
    }

    virtual std::size_t read(char *ptr, std::size_t size, int *ec, std::string *emsg) override {
        boost::system::error_code bec;
        std::size_t rd = boost::asio::read(m_sock, boost::asio::buffer(ptr, size), bec);

        if (ec) *ec = bec.value();
        if (emsg) *emsg = bec.message();

        return rd;
    }
    virtual std::size_t read_some(char *ptr, std::size_t size, int *ec, std::string *emsg) override {
        boost::system::error_code bec;
        std::size_t rd = m_sock.read_some(boost::asio::buffer(ptr, size), bec);

        if (ec) *ec = bec.value();
        if (emsg) *emsg = bec.message();

        return rd;
    }
    virtual void async_read(char *buf, std::size_t size, on_read_handler cb, impl_holder holder) override {
        boost::asio::async_read(
             m_sock
            ,boost::asio::buffer(buf, size)
            ,make_preallocated_handler(
                 m_read_allocator
                ,[this, buf, cb, holder]
                 (const boost::system::error_code &ec, std::size_t rd)
                 { cb(std::move(holder), __NETIO_MAKE_FILELINE(), ec.value(), ec.message().c_str(), buf, rd); }
            )
        );
    }
    virtual void async_read_some(char *buf, std::size_t size, on_read_handler cb, impl_holder holder) override {
        m_sock.async_read_some(
            boost::asio::buffer(buf, size)
            ,make_preallocated_handler(
                m_read_allocator
                ,[this, buf, cb, holder]
                 (const boost::system::error_code &ec, std::size_t rd)
                 { cb(std::move(holder), __NETIO_MAKE_FILELINE(), ec.value(), ec.message().c_str(), buf, rd); }
            )
        );
    }

    virtual std::size_t write(const char *ptr, std::size_t size, int *ec, std::string *emsg) override {
        boost::system::error_code bec;
        std::size_t wr = boost::asio::write(m_sock, boost::asio::buffer(ptr, size), bec);

        if (ec) *ec = bec.value();
        if (emsg) *emsg = bec.message();

        return wr;
    }
    virtual std::size_t write_some(const char *ptr, std::size_t size, int *ec, std::string *emsg) override {
        boost::system::error_code bec;
        std::size_t wr = m_sock.write_some(boost::asio::buffer(ptr, size), bec);

        if (ec) *ec = bec.value();
        if (emsg) *emsg = bec.message();

        return wr;
    }
    virtual void async_write(const char *buf, std::size_t size, on_write_handler cb, impl_holder holder) override {
        boost::asio::async_write(
             m_sock
            ,boost::asio::buffer(buf, size)
            ,make_preallocated_handler(
                 m_write_allocator
                ,[this, buf, cb, holder]
                 (const boost::system::error_code &ec, std::size_t wr)
                 { cb(std::move(holder), __NETIO_MAKE_FILELINE(), ec.value(), ec.message().c_str(), buf, wr); }
            )
        );
    }
    virtual void async_write_some(const char *buf, std::size_t size, on_write_handler cb, impl_holder holder) override {
        m_sock.async_write_some(
            boost::asio::buffer(buf, size)
            ,make_preallocated_handler(
                m_write_allocator
                ,[this, buf, cb, holder]
                 (const boost::system::error_code &ec, std::size_t wr)
                 { cb(std::move(holder), __NETIO_MAKE_FILELINE(), ec.value(), ec.message().c_str(), buf, wr); }
            )
        );
    }

private:
    worker &m_worker;
    handler_allocator<256> m_read_allocator;
    handler_allocator<256> m_write_allocator;
    boost::asio::ip::tcp::socket m_sock;
}; // struct asio_socket

socket_type create_socket(worker &w) {
    return std::make_shared<asio_socket>(w);
}

/**************************************************************************/
/**************************************************************************/
/**************************************************************************/

struct asio_acceptor: i_acceptor {
    asio_acceptor(worker &w, const char *ip, uint16_t port)
        :i_acceptor{}
        ,m_worker{w}
        ,m_accept_allocator{}
        ,m_acc{
            *static_cast<boost::asio::io_service *>(m_worker.get_handle())
            ,boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ip), port)
        }
    {}

    virtual ~asio_acceptor() { close(); }

    virtual worker& get_worker() const override { return m_worker; }

    virtual socket_type accept(int *ec, std::string *emsg) override {
        socket_type sock = create_socket(m_worker);
        boost::asio::ip::tcp::socket *asock = static_cast<boost::asio::ip::tcp::socket *>(sock->get_handle());

        boost::system::error_code bec;
        m_acc.accept(*asock, bec);

        if (ec) *ec = bec.value();
        if (emsg) *emsg = bec.message();

        return sock;
    }
    virtual void async_accept(on_accept_handler cb, impl_holder holder) override {
        socket_type sock = create_socket(get_worker());

        async_accept(
             std::move(sock)
            ,[this, cb]
             (impl_holder holder, const char *fl, int ec, const char *emsg, socket_type sock)
             { cb(std::move(holder), fl, ec, emsg, std::move(sock)); }
            ,std::move(holder)
        );
    }

    virtual void async_accept(socket_type sock, on_accept_handler cb, impl_holder holder) override {
        boost::asio::ip::tcp::socket *asock = static_cast<boost::asio::ip::tcp::socket *>(sock->get_handle());
        m_acc.async_accept(
            *asock
            ,make_preallocated_handler(
                 m_accept_allocator
                ,[this, cb, sock, holder]
                 (const boost::system::error_code &ec)
                 { cb(std::move(holder), __NETIO_MAKE_FILELINE(), ec.value(), ec.message().c_str(), std::move(sock)); }
            )
        );
    }

    virtual void close() override {
        boost::system::error_code ec;
        m_acc.close(ec);
    }

private:
    worker &m_worker;
    handler_allocator<256> m_accept_allocator;
    boost::asio::ip::tcp::acceptor m_acc;
}; // struct asio_acceptor

/**************************************************************************/

acceptor_type create_acceptor(worker &w, const char *ip, uint16_t port) {
    return std::make_shared<asio_acceptor>(w, ip, port);
}

/**************************************************************************/
/**************************************************************************/
/**************************************************************************/

struct asio_one_shot_timer: i_timer {
    asio_one_shot_timer(worker &w)
        :i_timer{}
        ,m_worker{w}
        ,m_timer{*static_cast<boost::asio::io_service *>(m_worker.get_handle())}
        ,m_started{false}
    {}

    virtual worker& get_worker() override { return m_worker; }

    virtual void start(std::size_t ms, on_timeout_handler cb, impl_holder holder) override {
        if ( m_started ) {
            cb(std::move(holder), __NETIO_MAKE_FILELINE(), -1, "Timer already started");

            return;
        }

        m_started = true;

        m_timer.expires_from_now(std::chrono::milliseconds(ms));
        m_timer.async_wait(
            [this, cb, holder]
            (const boost::system::error_code &ec) {
                m_started = false;
                cb(std::move(holder), __NETIO_MAKE_FILELINE(), ec.value(), ec.message().c_str());
            }
        );
    }

    virtual void stop() override {
        m_started = false;
        m_timer.cancel();
    }

    virtual bool started() const override { return m_started; }

    worker &m_worker;
    boost::asio::steady_timer m_timer;
    bool m_started;
};

/**************************************************************************/

struct asio_periodic_timer: i_timer {
    asio_periodic_timer(worker &w)
        :i_timer{}
        ,m_worker{w}
        ,m_timer{*static_cast<boost::asio::io_service *>(m_worker.get_handle())}
        ,m_started{false}
    {}

    virtual worker& get_worker() override { return m_worker; }

    void on_timeout(const boost::system::error_code &ec, std::size_t ms, on_timeout_handler cb, impl_holder holder) {
        cb(holder, __NETIO_MAKE_FILELINE(), ec.value(), ec.message().c_str());

        if ( ec )  {
            m_started = false;
            return;
        }

        if ( !m_started )
            return;

        m_timer.expires_from_now(std::chrono::milliseconds(ms));
        m_timer.async_wait(
            [this, ms, cb, holder]
            (const boost::system::error_code &ec)
            { on_timeout(ec, ms, std::move(cb), std::move(holder)); }
        );
    }

    virtual void start(std::size_t ms, on_timeout_handler cb, impl_holder holder) override {
        if ( m_started ) {
            cb(std::move(holder), __NETIO_MAKE_FILELINE(), -1, "Timer already started");

            return;
        }

        m_started = true;

        m_timer.expires_from_now(std::chrono::milliseconds(ms));
        m_timer.async_wait(
            [this, ms, cb, holder]
            (const boost::system::error_code &ec)
            { on_timeout(ec, ms, std::move(cb), std::move(holder)); }
        );
    }

    virtual void stop() override {
        m_started = false;
        m_timer.cancel();
    }

    virtual bool started() const override { return m_started; }

    worker &m_worker;
    boost::asio::steady_timer m_timer;
    bool m_started;
};

/**************************************************************************/

timer_type create_one_shot_timer(worker &w) {
    return std::make_shared<asio_one_shot_timer>(w);
}

timer_type create_periodic_timer(worker &w) {
    return std::make_shared<asio_periodic_timer>(w);
}

/**************************************************************************/
/**************************************************************************/
/**************************************************************************/

struct worker::impl {
    impl(void *impl_data)
        :m_impl_data{impl_data}
    {
        if ( !m_impl_data ) {
            m_ios.reset(new boost::asio::io_service);
            m_impl_data = m_ios.get();
        }
    }
    ~impl()
    {}

    void run() {
        static_cast<boost::asio::io_service*>(m_impl_data)->run();
    }

    void post(std::function<void()> f) {
        static_cast<boost::asio::io_service*>(m_impl_data)->post(std::move(f));
    }

    std::unique_ptr<boost::asio::io_service> m_ios;
    void *m_impl_data;
};

/**************************************************************************/

worker::worker(void *impl_data)
    :pimpl(new impl(impl_data))
{}

worker::~worker()
{ delete pimpl; }

/**************************************************************************/

void worker::run() { pimpl->run(); }

void worker::post(std::function<void()> f) { pimpl->post(std::move(f)); }

void* worker::get_handle() { return pimpl->m_impl_data; }

/**************************************************************************/
/**************************************************************************/
/**************************************************************************/

} // ns net
} // ns gbp
