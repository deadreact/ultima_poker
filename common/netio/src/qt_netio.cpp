
#include <netio/ppdefs.hpp>
#include <netio/timer.hpp>
#include <netio/netio.hpp>
#include <netio/handler_allocator.hpp>
#include <netio/preallocated_handler_invoker.hpp>
#include <netio/make_preallocated_handler.hpp>

#include <QtCore/QTimer>

#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>

#include <iostream>

namespace gbp {
namespace net {

/**************************************************************************/

struct qt_socket: i_socket {
    qt_socket(worker &w)
		:i_socket()
        ,m_worker(w)
        ,m_sock(*static_cast<boost::asio::io_service*>(m_worker.get_handle()))
        ,m_read_allocator{}
        ,m_write_allocator{}
	{}

	virtual ~qt_socket() {
		close();
	}

    virtual worker& get_worker() override { return m_worker; }
    virtual void* get_handle() override { return &m_sock; }

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
             (const boost::system::error_code &ec){
                cb(std::move(holder), __NETIO_MAKE_FILELINE(), ec.value(), ec.message().c_str());
            }
        );
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
                    (const boost::system::error_code &ec, std::size_t rd) {
                    cb(std::move(holder), __NETIO_MAKE_FILELINE(), ec.value(), ec.message().c_str(), buf, rd);
                }
            )
        );
    }
    virtual void async_read_some(char *buf, std::size_t size, on_read_handler cb, impl_holder holder) override {
        m_sock.async_read_some(
             boost::asio::buffer(buf, size)
            ,make_preallocated_handler(
                 m_read_allocator
                ,[this, buf, cb, holder]
                 (const boost::system::error_code &ec, std::size_t rd) {
                    cb(std::move(holder), __NETIO_MAKE_FILELINE(), ec.value(), ec.message().c_str(), buf, rd);
                }
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
                    (const boost::system::error_code &ec, std::size_t wr) {
                    cb(std::move(holder), __NETIO_MAKE_FILELINE(), ec.value(), ec.message().c_str(), buf, wr);
                }
            )
        );
    }
    virtual void async_write_some(const char *buf, std::size_t size, on_write_handler cb, impl_holder holder) override {
        m_sock.async_write_some(
             boost::asio::buffer(buf, size)
            ,make_preallocated_handler(
                 m_write_allocator
                ,[this, buf, cb, holder]
                 (const boost::system::error_code &ec, std::size_t wr) {
                    cb(std::move(holder), __NETIO_MAKE_FILELINE(), ec.value(), ec.message().c_str(), buf, wr);
                }
            )
        );
    }


    worker &m_worker;
	boost::asio::ip::tcp::socket m_sock;
    handler_allocator<256> m_read_allocator;
    handler_allocator<256> m_write_allocator;
}; // struct qt_socket

/**************************************************************************/

socket_type create_socket(worker &w) {
    socket_type sock(new qt_socket(w));

	return sock;
}

/**************************************************************************/
/**************************************************************************/
/**************************************************************************/

struct qt_one_shot_timer: i_timer {
    qt_one_shot_timer(worker &w)
        :i_timer{}
        ,m_worker{w}
        ,m_timer{*static_cast<boost::asio::io_service*>(m_worker.get_handle())}
        ,m_started{false}
    {}

    void on_timeout(const char *fl, int ec, const char *emsg, on_timeout_handler cb, impl_holder holder) {
        m_started = false;
        cb(std::move(holder), fl, ec, emsg);
    }

    virtual worker& get_worker() override { return m_worker; }

    virtual void start(std::size_t ms, on_timeout_handler cb, impl_holder holder) override {
        if ( m_started ) {
            cb(std::move(holder), __NETIO_MAKE_FILELINE(), -1, "Timer already started");

            return;
        }
        m_started = true;

        m_timer.expires_from_now(std::chrono::milliseconds(ms));
        m_timer.async_wait(
            [this, cb, holder](const boost::system::error_code &ec) {
                on_timeout(__NETIO_MAKE_FILELINE(), ec.value(), ec.message().c_str(), std::move(cb), std::move(holder));
            }
        );
    }

    virtual void stop() override {
        m_started = false;
        m_timer.cancel();
    }

    virtual bool started() const override { return m_started; }

private:
    worker &m_worker;
    boost::asio::steady_timer m_timer;
    bool m_started;
};

/**************************************************************************/

struct qt_periodic_timer: i_timer {
    qt_periodic_timer(worker &w)
        :i_timer{}
        ,m_worker{w}
        ,m_started{false}
    {}

    virtual worker& get_worker() override { return m_worker; }

    virtual void start(std::size_t ms, on_timeout_handler cb, impl_holder holder) override {
    	(void)ms;
    	(void)cb;
    	(void)holder;
    	
        assert(0);
    }

    virtual void stop() override {}

    virtual bool started() const override { return false; }

private:
    worker &m_worker;
    bool m_started;
};

/**************************************************************************/

timer_type create_one_shot_timer(worker &w) {
    return std::make_shared<qt_one_shot_timer>(w);
}

timer_type create_periodic_timer(worker &w) {
    return std::make_shared<qt_periodic_timer>(w);
}

/**************************************************************************/
/**************************************************************************/
/**************************************************************************/

#if 0
struct worker::impl {
    impl(void *impl_data)
        :m_impl_data{impl_data ? impl_data : nullptr}
        ,m_ios{1}
        ,m_thread{}
    {
        MyThread mth;
        mth.start();
        mth.wait();
    }
    ~impl() {
        if ( m_thread.joinable() ) {
            m_thread.join();
        }

        m_ios.run();
    }

    void run() {
        m_thread = std::thread(
            [this](){
                m_ios.run();
            }
        );
    }

    void post(std::function<void()> f) {
        m_ios.post(std::move(f));
    }

    void stop() {
        m_ios.stop();

        if ( m_thread.joinable() ) {
            m_thread.join();
        }
        m_ios.run();
    }

    void wait() {
        if ( m_thread.joinable() ) {
            m_thread.join();
        }
        m_ios.run();
    }

    void *m_impl_data;
    boost::asio::io_service m_ios;
    std::thread m_thread;
};
#endif

struct worker::impl {
    impl(void *impl_data)
        :m_impl_data{impl_data}
        ,m_timer{static_cast<QObject*>(impl_data)}
        ,m_ios{1}
    {
        QObject::connect(&m_timer, &QTimer::timeout,
            [this](){
                try {
                    boost::system::error_code ec;
                    m_ios.poll_one(ec);

                    if ( ec ) {
                        std::cerr << "qt_socket::poll_one(): " << __NETIO_MAKE_FILELINE() << ": ec=" << ec.value() << ", msg=" << ec.message() << std::endl;
                    }
                } catch (const std::exception &ex) {
                    std::cerr << "qt_socket::poll_one(): std::exception: " << __NETIO_MAKE_FILELINE() << ": " << ex.what() << std::endl;
                } catch (...) {
                    std::cerr << "qt_socket::poll_one(): unknown exception: " << __NETIO_MAKE_FILELINE() << ": " << std::endl;
                }
            }
        );
        m_timer.start(5);
    }
    ~impl()
    {}

    void run()
    {}

    void post(std::function<void()> f) {
        m_ios.post(std::move(f));
    }

    void *m_impl_data;
    QTimer m_timer;
    boost::asio::io_service m_ios;
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

void* worker::get_handle() { return &(pimpl->m_ios); }

/**************************************************************************/
/**************************************************************************/
/**************************************************************************/

} // ns net
} // ns gbp
