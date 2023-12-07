
#include <netio/distributor.hpp>
#include <netio/cryptotools.hpp>
#include <netio/ppdefs.hpp>
#include <netio/netio.hpp>
#include <netio/timer.hpp>

#include <boost/asio/ip/tcp.hpp>

#include <deque>
#include <cstring>
# include <iostream>

/**************************************************************************/

//#define NETIO_DEBUG
#define NETIO_DEBUG_CLIENT 1
#define NETIO_DEBUG_SERVER 0

#ifdef NETIO_DEBUG
# include <iostream>
# define NETIO_DEBUG_EXPR(s, ...) { \
    std::cout << (s == NETIO_DEBUG_CLIENT ? "m_netio::client" : "m_netio::server") << "(" << this << ")(" << __FUNCTION__ << "): " \
        << __VA_ARGS__ << std::flush; \
}
#else
# define NETIO_DEBUG_EXPR(s, ...)
#endif // NETIO_DEBUG

/**************************************************************************/

//#define NETIO_DEBUG_INACTIVITY_TIMER
#ifdef NETIO_DEBUG_INACTIVITY_TIMER
# include <iostream>
static const char *__netio_inactivity_states[] = {
     "write handshake"
    ,"handshake writen"
    ,"read handshake"
    ,"handshake readed"
    ,"read header"
    ,"header readed"
    ,"read body"
    ,"body readed"
    ,"null"
};
# define NETIO_SHOW_INACTIVITY_TIMER_STATE(this, state, fl, ec, emsg) \
    std::cout \
    << "session: " << this \
    << ", startfl=" << fl \
    << ", state=" << __netio_inactivity_states[static_cast<unsigned>(state)] \
    << ", ec=" << (ec ? ec : error::timed_out) \
    << ", emsg=" << (!ec ? boost::system::error_code(boost::asio::error::timed_out).message().c_str() : emsg) \
    << std::endl;
#else
# define NETIO_SHOW_INACTIVITY_TIMER_STATE(this, state, fl, ec, emsg)
#endif // NETIO_DEBUG_INACTIVITY_TIMER

/**************************************************************************/

#if defined(NETIO_DEBUG_INACTIVITY_TIMER) || defined(NETIO_DEBUG)
# define NETIO_EXPAND_EXPR(...) { __VA_ARGS__ }
#else
# define NETIO_EXPAND_EXPR(...)
#endif // defined(NETIO_DEBUG_INACTIVITY_TIMER) || defined(NETIO_DEBUG)

/**************************************************************************/

namespace gbp {
namespace net {

/**************************************************************************/

struct netio::impl {
    impl(
         worker &w
        ,std::size_t flags
        ,std::size_t inactivity_timeout
        ,std::size_t max_body_size
        ,std::size_t compress_if_greater
        ,on_error_handler eh
        ,on_read_handler rh
        ,on_write_handler wh
        ,on_incoming_handler ih
        ,on_netio_stop_handler sh
    )
        :m_worker{w}
        ,m_sock{create_socket(m_worker)}
        ,m_flags{flags}
        ,m_ip{}
        ,m_port{}
        ,m_inactivity_timeout{inactivity_timeout}
        ,m_inactivity_timer{(m_flags & test_inactivity) ? create_one_shot_timer(m_worker) : nullptr }
        ,m_max_body_size{max_body_size}
        ,m_error_handler{std::move(eh)}
        ,m_read_handler{std::move(rh)}
        ,m_write_handler{std::move(wh)}
        ,m_read_some_handler{}
        ,m_write_some_handler{}
        ,m_incoming_handler{std::move(ih)}
        ,m_netio_stop_handler{std::move(sh)}
        ,m_distributor{}
        ,m_crypto{compress_if_greater}
        ,m_tmp_pk_and_nonce{}
        ,m_handshake_finished{false}
        ,m_write_in_process{false}
        ,m_stop_requested{false}
        ,m_header{}
        ,m_body_buf{}
        ,m_body_buf_size{}
        ,m_buffers{}
        ,m_serv{}
    {}

    impl(
         socket_type sock
        ,std::size_t flags
        ,std::size_t inactivity_timeout
        ,std::size_t max_body_size
        ,std::size_t compress_if_greater
        ,on_error_handler eh
        ,on_read_handler rh
        ,on_write_handler wh
        ,on_incoming_handler ih
        ,on_netio_stop_handler sh
    )
        :m_worker{sock->get_worker()}
        ,m_sock{std::move(sock)}
        ,m_flags{flags}
        ,m_ip{}
        ,m_port{}
        ,m_inactivity_timeout{inactivity_timeout}
        ,m_inactivity_timer{(m_flags & test_inactivity) ? create_one_shot_timer(m_worker) : nullptr }
        ,m_max_body_size{max_body_size}
        ,m_error_handler{std::move(eh)}
        ,m_read_handler{std::move(rh)}
        ,m_write_handler{std::move(wh)}
        ,m_incoming_handler{std::move(ih)}
        ,m_netio_stop_handler{std::move(sh)}
        ,m_distributor{}
        ,m_crypto{compress_if_greater}
        ,m_tmp_pk_and_nonce{}
        ,m_handshake_finished{false}
        ,m_write_in_process{false}
        ,m_stop_requested{false}
        ,m_header{}
        ,m_body_buf{}
        ,m_body_buf_size{}
        ,m_buffers{}
        ,m_serv{}
    {}

    ~impl() {
        stop();
    }

    std::size_t flags() const { return m_flags; }
    bool is_client_side() const { return static_cast<bool>(flags() & client); }
    bool is_server_side() const { return static_cast<bool>(flags() & server); }
    bool is_no_crypt() const { return static_cast<bool>(flags() & no_crypt); }

    void start_services(impl_holder holder) {
        for ( auto &it: m_serv ) {
            if ( !it.second->started() ) {
                it.second->start(holder);
            }
        }

    }
    void stop_services() {
        for ( auto it = m_serv.rbegin(); it != m_serv.rend(); ++it ) {
            if ( it->second->started() ) {
                it->second->stop();
            }
        }
    }

    void start(impl_holder holder) {
        if ( !m_sock->is_open() ) {
            if ( m_error_handler ) {
                __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                    m_error_handler(
                         std::move(holder)
                        ,__NETIO_MAKE_FILELINE()
                        ,-1
                        ,"Socket is not connected"
                    );
                )
            }

            return;
        }
        if ( !is_no_crypt() && !m_handshake_finished ) {
            if ( m_error_handler ) {
                __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                    m_error_handler(
                         std::move(holder)
                        ,__NETIO_MAKE_FILELINE()
                        ,-1
                        ,"Handshake not finished"
                    );
                )
            }

            return;
        }

        m_stop_requested = false;

        read_header(holder);
        start_services(std::move(holder));
    }
    void stop() {
        stop_services();

        m_stop_requested = true;
        m_handshake_finished = false;
        close_or_realloc_socket();

        if ( m_flags & test_inactivity ) {
            m_inactivity_timer->stop();
        }
    }
    void close_or_realloc_socket() {
        const int ec = m_sock->close();
        if ( ec ) {
            m_sock = create_socket(m_worker);
        }
    }

    void async_connect(timer_type timer, const char *ip, std::uint16_t port, on_connect_handler cb, impl_holder holder) {
        m_ip = ip;
        m_port = port;

        if ( !timer ) {
            timer = create_one_shot_timer(m_sock->get_worker());
        }

        if ( m_sock->is_open() ) {
            close_or_realloc_socket();
        }

        m_sock->async_connect(
             m_ip.c_str()
            ,m_port
            ,[this, timer, cb]
             (impl_holder holder, const char *fl, int ec, const char *emsg)
             {on_connected(std::move(holder), std::move(timer), std::move(cb), fl, ec, emsg);}
            ,std::move(holder)
        );
    }
    void on_connected(impl_holder holder, timer_type timer, on_connect_handler cb, const char *fl, int ec, const char *emsg) {
        if ( ec == error::connection_refused || ec == boost::asio::error::network_unreachable ) {

            timer->start(
                 1000
                ,[this, timer, cb]
                 (impl_holder holder, const char */*fl*/, int /*ec*/, const char */*emsg*/)
                 {async_connect(std::move(timer), m_ip.c_str(), m_port, std::move(cb), std::move(holder));}
                ,std::move(holder)
            );

            return;
        }

        cb(std::move(holder), fl, ec, emsg);
    }

    // inactivity timer
    enum class inactivity_type {
         start_write_handshake
        ,handshake_writen
        ,start_read_handshake
        ,handshake_readed
        ,start_read_header
        ,header_readed
        ,start_read_body
        ,body_readed
    };
    void inactivity_timer_restart(impl_holder holder, const char *startfl, inactivity_type itype) {
        if ( m_inactivity_timer->started() ) {
            m_inactivity_timer->stop();
        }

        m_inactivity_timer->start(
            m_inactivity_timeout*1000
            ,[this, startfl, itype]
             (impl_holder holder, const char *fl, int ec, const char *emsg)
             { on_inactivity_timer_timedout(std::move(holder), fl, ec, emsg, startfl, itype); }
            ,std::move(holder)
        );
    }
    void on_inactivity_timer_timedout(impl_holder holder, const char *fl, int ec, const char *emsg, const char *startfl, inactivity_type itype) {
        (void)fl;
        (void)emsg;
        (void)startfl;
        (void)itype;

        NETIO_SHOW_INACTIVITY_TIMER_STATE(this, itype, startfl, ec, emsg)

        if ( ec == error::operation_aborted ) {
            return;
        }

        if ( m_error_handler ) {
            __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                m_error_handler(
                     std::move(holder)
                    ,__NETIO_MAKE_FILELINE()
                    ,error::timed_out
                    ,boost::system::error_code(boost::asio::error::timed_out).message().c_str()
                );
            )
        }

        m_stop_requested = true;

        stop();
    }

    // ************************ client handshake
    void async_handshake_as_client(on_handshake_handler hh, impl_holder holder) {
        m_tmp_pk_and_nonce = m_crypto.get_own_piblic_key_and_nonce();

        NETIO_DEBUG_EXPR(
            NETIO_DEBUG_CLIENT, "async write pk and nonce:\n"
            << gbp::net::hexdump(m_tmp_pk_and_nonce.data(), m_tmp_pk_and_nonce.size()) << std::endl
        )

        if ( m_flags & test_inactivity ) {
            inactivity_timer_restart(holder, __NETIO_MAKE_FILELINE(), inactivity_type::start_write_handshake);
        }

        m_sock->async_write(
             reinterpret_cast<const char *>(m_tmp_pk_and_nonce.data())
            ,m_tmp_pk_and_nonce.size()
            ,[this, hh]
             (impl_holder holder, const char *fl, int ec, const char *emsg, const char *buf, std::size_t size)
             { async_as_client_handshake_writen(std::move(hh), std::move(holder), fl, ec, emsg, buf, size); }
            ,std::move(holder)
        );
    }
    void async_as_client_handshake_writen(on_handshake_handler hh, impl_holder holder, const char *fl, int ec, const char *emsg, const char *, std::size_t) {
        (void)fl;

        NETIO_DEBUG_EXPR(NETIO_DEBUG_CLIENT, "async pk and nonce writen, ec=" << ec << ", emsg=" << emsg << std::endl)

        if ( ec ) {
            stop_services();
            m_handshake_finished = false;
            close_or_realloc_socket();

            if ( m_error_handler ) {
                __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                    m_error_handler(
                         holder
                        ,__NETIO_MAKE_FILELINE()
                        ,ec
                        ,emsg
                    );
                )
            }
            if ( hh ) {
                __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                    hh(
                         holder
                        ,__NETIO_MAKE_FILELINE()
                        ,ec
                        ,emsg
                    );
                )
            }

            return;
        }

        std::memset(m_tmp_pk_and_nonce.data(), 0, m_tmp_pk_and_nonce.size());

        if ( m_flags & test_inactivity ) {
            inactivity_timer_restart(holder, __NETIO_MAKE_FILELINE(), inactivity_type::handshake_writen);
        }
        if ( m_flags & test_inactivity ) {
            inactivity_timer_restart(holder, __NETIO_MAKE_FILELINE(), inactivity_type::start_read_handshake);
        }

        NETIO_DEBUG_EXPR(NETIO_DEBUG_CLIENT, "async read pk and nonce" << std::endl)
        m_sock->async_read(
             reinterpret_cast<char *>(m_tmp_pk_and_nonce.data())
            ,m_tmp_pk_and_nonce.size()
            ,[this, hh]
             (impl_holder holder, const char *fl, int ec, const char *emsg, char *buf, std::size_t size)
             { async_as_client_handshake_readed(std::move(hh), std::move(holder), fl, ec, emsg, buf, size); }
            ,std::move(holder)
        );
    }
    void async_as_client_handshake_readed(on_handshake_handler hh, impl_holder holder, const char *fl, int ec, const char *emsg, char *, std::size_t) {
        (void)fl;

        NETIO_DEBUG_EXPR(
             NETIO_DEBUG_CLIENT
            ,"async pk and nonce readed, ec=" << ec << ", emsg=" << emsg << ":\n"
                                              << gbp::net::hexdump(m_tmp_pk_and_nonce.data(), m_tmp_pk_and_nonce.size()) << std::endl
        )

        if ( ec ) {
            stop_services();
            m_handshake_finished = false;
            close_or_realloc_socket();

            if ( m_error_handler ) {
                __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                    m_error_handler(
                         holder
                        ,__NETIO_MAKE_FILELINE()
                        ,ec
                        ,emsg
                    );
                )
            }
            if ( hh ) {
                __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                    hh(
                         holder
                        ,__NETIO_MAKE_FILELINE()
                        ,ec
                        ,emsg
                    );
                )
            }

            return;
        }

        if ( m_flags & test_inactivity ) {
            inactivity_timer_restart(holder, __NETIO_MAKE_FILELINE(), inactivity_type::handshake_readed);
        }

        m_crypto.set_oppo_public_key_and_nonce(m_tmp_pk_and_nonce);

        try {
            m_crypto.generate_shared_key(cryptotools::client_side);
        } catch (const std::exception &ex) {
            stop_services();
            m_handshake_finished = false;
            close_or_realloc_socket();

            std::string emsg = "client handshake error: std::exception: ";
            emsg += ex.what();
            if ( m_error_handler ) {
                __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                    m_error_handler(
                         holder
                        ,__NETIO_MAKE_FILELINE()
                        ,-1
                        ,emsg.c_str()
                    );
                )
            }
            if ( hh ) {
                __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                    hh(
                         holder
                        ,__NETIO_MAKE_FILELINE()
                        ,-1
                        ,emsg.c_str()
                    );
                )
            }

            return;
        } catch (...) {
            stop_services();
            m_handshake_finished = false;
            close_or_realloc_socket();

            static const char *em = "client handshake error: unknown exception";
            if ( m_error_handler ) {
                __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                    m_error_handler(
                         holder
                        ,__NETIO_MAKE_FILELINE()
                        ,-1
                        ,em
                    );
                )
            }
            if ( hh ) {
                __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                    hh(
                         holder
                        ,__NETIO_MAKE_FILELINE()
                        ,-1
                        ,em
                    );
                )
            }

            return;
        }

        m_handshake_finished = true;

        if ( hh ) {
            __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                hh(
                     std::move(holder)
                    ,nullptr
                    ,0
                    ,nullptr
                );
            )
        }

        NETIO_DEBUG_EXPR(
             NETIO_DEBUG_CLIENT
            ,"async shared key:\n" << gbp::net::hexdump(m_crypto.get_shared_key(), m_crypto.shared_key_size()) << std::endl
        )
    }

    // blocked
    void handshake_as_client(int *ec, std::string *emsg) {
        m_tmp_pk_and_nonce = m_crypto.get_own_piblic_key_and_nonce();

        NETIO_DEBUG_EXPR(
             NETIO_DEBUG_CLIENT
            ,"sync write pk and nonce:\n" << gbp::net::hexdump(m_tmp_pk_and_nonce.data(), m_tmp_pk_and_nonce.size()) << std::endl
        )

        if ( ec ) *ec = 0;
        if ( emsg ) emsg->clear();

        int lec=0;
        std::string lemsg;

        m_sock->write(reinterpret_cast<const char *>(m_tmp_pk_and_nonce.data()), m_tmp_pk_and_nonce.size(), &lec, &lemsg);

        NETIO_DEBUG_EXPR(NETIO_DEBUG_CLIENT, "sync pk and nonce writen, ec=" << lec << ", emsg=" << lemsg << std::endl)

        if ( lec ) {
            stop_services();
            m_handshake_finished = false;
            close_or_realloc_socket();

            if ( ec ) *ec = lec;
            if ( emsg ) *emsg = lemsg;

            return;
        }

        std::memset(m_tmp_pk_and_nonce.data(), 0, m_tmp_pk_and_nonce.size());

        NETIO_DEBUG_EXPR(NETIO_DEBUG_CLIENT, "sync read pk and nonce" << std::endl)

        m_sock->read(reinterpret_cast<char *>(m_tmp_pk_and_nonce.data()), m_tmp_pk_and_nonce.size(), &lec, &lemsg);

        NETIO_DEBUG_EXPR(
             NETIO_DEBUG_CLIENT
            ,"async pk and nonce readed, ec=" << lec << ", emsg=" << lemsg << ":\n"
                                              << gbp::net::hexdump(m_tmp_pk_and_nonce.data(), m_tmp_pk_and_nonce.size()) << std::endl
        )

        if ( lec ) {
            stop_services();
            m_handshake_finished = false;
            close_or_realloc_socket();

            if ( ec ) *ec = lec;
            if ( emsg ) *emsg = lemsg;

            return;
        }

        m_crypto.set_oppo_public_key_and_nonce(m_tmp_pk_and_nonce);

        try {
            m_crypto.generate_shared_key(cryptotools::client_side);
            m_handshake_finished = true;
        } catch (const std::exception &ex) {
            stop_services();
            m_handshake_finished = false;
            close_or_realloc_socket();

            lemsg = "client handshake error: std::exception: ";
            lemsg += ex.what();

            if ( ec ) *ec = -1;
            if ( emsg ) *emsg = lemsg;

            return;
        } catch (...) {
            stop_services();
            m_handshake_finished = false;
            close_or_realloc_socket();

            if ( ec ) *ec = -1;
            if ( emsg ) *emsg = "client handshake error: unknown exception";

            return;
        }

        NETIO_DEBUG_EXPR(
             NETIO_DEBUG_CLIENT
            ,"sync shared key:\n" << gbp::net::hexdump(m_crypto.get_shared_key(), m_crypto.shared_key_size()) << std::endl
        )
    }

    // ************************ server handshake
    // -> 0
    void async_handshake_as_server(on_handshake_handler hh, impl_holder holder) {
        m_tmp_pk_and_nonce = m_crypto.get_own_piblic_key_and_nonce();

        NETIO_DEBUG_EXPR(NETIO_DEBUG_SERVER, "async read pk and nonce" << std::endl)

        if ( m_flags & test_inactivity ) {
            inactivity_timer_restart(holder, __NETIO_MAKE_FILELINE(), inactivity_type::start_read_handshake);
        }

        m_sock->async_read(
             reinterpret_cast<char *>(m_tmp_pk_and_nonce.data())
            ,m_tmp_pk_and_nonce.size()
            ,[this, hh]
             (impl_holder holder, const char *fl, int ec, const char *emsg, char *buf, std::size_t size)
             { async_as_server_handshake_readed(std::move(hh), std::move(holder), fl, ec, emsg, buf, size); }
            ,std::move(holder)
        );
    }

    // 0 -> 1
    void async_as_server_handshake_readed(on_handshake_handler hh, impl_holder holder, const char *fl, int ec, const char *emsg, char *, std::size_t) {
        (void)fl;

        NETIO_DEBUG_EXPR(
             NETIO_DEBUG_SERVER
            ,"async pk and nonce readed, ec=" << ec << ", emsg=" << emsg << ":\n" << gbp::net::hexdump(m_tmp_pk_and_nonce.data(), m_tmp_pk_and_nonce.size()) << std::endl
        )

        if ( ec ) {
            stop_services();
            m_handshake_finished = false;
            close_or_realloc_socket();

            if ( !m_stop_requested ) {
                if (m_error_handler) {
                    __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                        m_error_handler(
                             holder
                            ,__NETIO_MAKE_FILELINE()
                            ,ec
                            ,emsg
                        );
                    )
                }
                if (hh) {
                    __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                        hh(
                             std::move(holder)
                            ,__NETIO_MAKE_FILELINE()
                            ,ec
                            ,emsg
                        );
                    )
                }
            } else {
                m_stop_requested = false;
            }

            return;
        }

        if ( m_flags & test_inactivity ) {
            inactivity_timer_restart(holder, __NETIO_MAKE_FILELINE(), inactivity_type::handshake_readed);
        }

        m_crypto.set_oppo_public_key_and_nonce(m_tmp_pk_and_nonce);

        try {
            m_crypto.generate_shared_key(cryptotools::server_side);
        } catch (const std::exception &ex) {
            stop_services();
            m_handshake_finished = false;
            close_or_realloc_socket();

            std::string emsg = "server handshake error: std::exception: ";
            emsg += ex.what();
            if ( m_error_handler ) {
                __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                    m_error_handler(
                         holder
                        ,__NETIO_MAKE_FILELINE()
                        ,-1
                        ,emsg.c_str()
                    );
                )
            }
            if ( hh ) {
                __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                    hh(
                         std::move(holder)
                        ,__NETIO_MAKE_FILELINE()
                        ,-1
                        ,emsg.c_str()
                    );
                )
            }

            return;
        } catch (...) {
            stop_services();
            m_handshake_finished = false;
            close_or_realloc_socket();

            static const char *em = "server handshake error: unknown exception";
            if ( m_error_handler ) {
                __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                    m_error_handler(
                         holder
                        ,__NETIO_MAKE_FILELINE()
                        ,-1
                        ,em
                    );
                )
            }
            if ( hh ) {
                __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                    hh(
                         std::move(holder)
                        ,__NETIO_MAKE_FILELINE()
                        ,-1
                        ,em
                    );
                )
            }

            return;
        }

        m_tmp_pk_and_nonce = m_crypto.get_own_piblic_key_and_nonce();

        NETIO_DEBUG_EXPR(
             NETIO_DEBUG_SERVER
            ,"async write pk and nonce:\n" << gbp::net::hexdump(m_tmp_pk_and_nonce.data(), m_tmp_pk_and_nonce.size()) << std::endl
        )

        if ( m_flags & test_inactivity ) {
            inactivity_timer_restart(holder, __NETIO_MAKE_FILELINE(), inactivity_type::start_write_handshake);
        }

        m_sock->async_write(
             reinterpret_cast<char *>(m_tmp_pk_and_nonce.data())
            ,m_tmp_pk_and_nonce.size()
            ,[this, hh]
             (impl_holder holder, const char *fl, int ec, const char *emsg, const char *buf, std::size_t size)
             { async_as_server_handshake_writen(std::move(hh), std::move(holder), fl, ec, emsg, buf, size); }
            ,std::move(holder)
        );
    }

    // 1 -> 2
    void async_as_server_handshake_writen(on_handshake_handler hh, impl_holder holder, const char *fl, int ec, const char *emsg, const char *, std::size_t) {
        (void)fl;

        NETIO_DEBUG_EXPR(NETIO_DEBUG_SERVER, "async pk and nonce writen, ec=" << ec << ", emsg=" << emsg << std::endl)

        if ( ec ) {
            stop_services();
            m_handshake_finished = false;
            close_or_realloc_socket();

            if ( m_error_handler ) {
                __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                    m_error_handler(
                         holder
                        ,__NETIO_MAKE_FILELINE()
                        ,ec
                        ,emsg
                    );
                )
            }
            if ( hh ) {
                __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                    hh(
                         std::move(holder)
                        ,__NETIO_MAKE_FILELINE()
                        ,ec
                        ,emsg
                    );
                )
            }

            return;
        }

        if ( m_flags & test_inactivity ) {
            inactivity_timer_restart(holder, __NETIO_MAKE_FILELINE(), inactivity_type::handshake_writen);
        }

        m_handshake_finished = true;

        if ( hh ) {
            __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                hh(
                     std::move(holder)
                    ,nullptr
                    ,0
                    ,nullptr
                );
            )
        }

        NETIO_DEBUG_EXPR(
             NETIO_DEBUG_SERVER
            ,"async shared key:\n" << gbp::net::hexdump(m_crypto.get_shared_key(), m_crypto.shared_key_size()) << std::endl
        )
    }

    // blocked
    void handshake_as_server(int *ec, std::string *emsg) {
        m_tmp_pk_and_nonce = m_crypto.get_own_piblic_key_and_nonce();

        if ( ec ) *ec = 0;
        if ( emsg ) emsg->clear();

        int lec=0;
        std::string lemsg;

        NETIO_DEBUG_EXPR(NETIO_DEBUG_SERVER, "sync read pk and nonce" << std::endl)

        m_sock->read(reinterpret_cast<char *>(m_tmp_pk_and_nonce.data()),m_tmp_pk_and_nonce.size(), &lec, &lemsg);

        NETIO_DEBUG_EXPR(
             NETIO_DEBUG_SERVER
            ,"sync pk and nonce readed, ec=" << lec << ", emsg=" << lemsg << ":\n"
                                             << gbp::net::hexdump(m_tmp_pk_and_nonce.data(), m_tmp_pk_and_nonce.size()) << std::endl
        )

        if ( lec ) {
            stop_services();
            m_handshake_finished = false;
            close_or_realloc_socket();

            if ( ec ) *ec = lec;
            if ( emsg ) *emsg = lemsg;

            return;
        }

        m_crypto.set_oppo_public_key_and_nonce(m_tmp_pk_and_nonce);

        try {
            m_crypto.generate_shared_key(cryptotools::server_side);
        } catch (const std::exception &ex) {
            stop_services();
            m_handshake_finished = false;
            close_or_realloc_socket();

            lemsg = "server handshake error: std::exception: ";
            lemsg += ex.what();

            if ( ec ) *ec = -1;
            if ( emsg ) *emsg = lemsg;

            return;
        } catch (...) {
            stop_services();
            m_handshake_finished = false;
            close_or_realloc_socket();

            if ( ec ) *ec = -1;
            if ( emsg ) *emsg = "server handshake error: unknown exception";

            return;
        }

        m_tmp_pk_and_nonce = m_crypto.get_own_piblic_key_and_nonce();

        NETIO_DEBUG_EXPR(
             NETIO_DEBUG_SERVER
            ,"sync write pk and nonce:\n" << gbp::net::hexdump(m_tmp_pk_and_nonce.data(), m_tmp_pk_and_nonce.size()) << std::endl
        )

        m_sock->write(reinterpret_cast<char *>(m_tmp_pk_and_nonce.data()), m_tmp_pk_and_nonce.size(), &lec, &lemsg);

        NETIO_DEBUG_EXPR(
             NETIO_DEBUG_SERVER
            ,"sync pk and nonce writen, ec=" << lec << ", emsg=" << lemsg << std::endl
        )

        if ( lec ) {
            stop_services();
            m_handshake_finished = false;
            close_or_realloc_socket();

            if ( ec ) *ec = lec;
            if ( emsg ) *emsg = lemsg;

            return;
        }

        m_handshake_finished = true;
    }

    /**************************************************************************/
    /**************************************************************************/
    /**************************************************************************/
    void read_header(impl_holder holder) {
        //std::cout << __PRETTY_FUNCTION__ << std::endl;
#ifdef NETIO_DEBUG
        std::cout << "m_netio::read_header()" << std::endl;
#endif // NETIO_DEBUG

        if ( m_flags & test_inactivity ) {
            inactivity_timer_restart(holder, __NETIO_MAKE_FILELINE(), inactivity_type::start_read_header);
        }

        const std::size_t to_read = sizeof(m_header);
        m_sock->async_read_some(
             reinterpret_cast<char *>(&m_header)
            ,to_read
            ,[this, to_read]
             (impl_holder holder, const char *fl, int ec, const char *emsg, char *buf, std::size_t size)
             { header_readed(std::move(holder), fl, ec, emsg, buf, to_read, size); }
            ,std::move(holder)
        );
    }

    void header_readed(impl_holder holder, const char *fl, int ec, const char *emsg, char *buf, std::size_t to_read, std::size_t readed) {
        (void)fl;

        if ( !ec && readed != to_read ) {
//            std::cout << "to_read=" << to_read << ", readed=" << readed << std::endl;
            to_read = to_read-readed;
            m_sock->async_read_some(
                 buf+readed
                ,to_read
                ,[this, to_read]
                 (impl_holder holder, const char *fl, int ec, const char *emsg, char *buf, std::size_t size)
                 { header_readed(std::move(holder), fl, ec, emsg, buf, to_read, size); }
                ,std::move(holder)
            );

            return;
        }

#ifdef NETIO_DEBUG
        std::cout << "m_netio::header_readed(): fl=" << fl << ", ec=" << ec << ", emsg=" << emsg << std::endl;
#endif // NETIO_DEBUG

        if ( ec ) {
            stop_services();
            m_handshake_finished = false;
            close_or_realloc_socket();

            if ( m_flags & test_inactivity ) {
                m_inactivity_timer->stop();
            }

            if ( !m_stop_requested ) {
                if ( m_error_handler ) {
                    __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                        m_error_handler(
                             std::move(holder)
                            ,__NETIO_MAKE_FILELINE()
                            ,ec
                            ,emsg
                        );
                    )
                }
            } else {
                m_stop_requested = false;
                if ( m_netio_stop_handler ) {
                    __NETIO_CATCH_AND_REPORT_EXCEPTIONS(m_netio_stop_handler(std::move(holder));)
                }
            }

            return;
        }

        if ( m_header > m_max_body_size ) {
            stop_services();
            m_handshake_finished = false;
            close_or_realloc_socket();

            if ( m_flags & test_inactivity ) {
                m_inactivity_timer->stop();
            }

            std::ostringstream os;
            os << "message body is too long(" << m_header << " bytes)";
            if ( m_error_handler ) {
                __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                    m_error_handler(
                         std::move(holder)
                        ,__NETIO_MAKE_FILELINE()
                        ,-1
                        ,os.str().c_str()
                    );
                )
            }

            return;
        }

        if ( m_flags & test_inactivity ) {
            inactivity_timer_restart(holder, __NETIO_MAKE_FILELINE(), inactivity_type::header_readed);
        }

        read_body(std::move(holder), m_header);
    }

    void read_body(impl_holder holder, std::uint32_t body_size) {
#ifdef NETIO_DEBUG
        std::cout << "m_netio::read_body(): body_size=" << body_size << std::endl;
#endif // NETIO_DEBUG

        if ( m_body_buf_size < body_size ) {
            m_body_buf.reset(new char[body_size]);
            m_body_buf_size = body_size;
        }

        if ( m_flags & test_inactivity ) {
            inactivity_timer_restart(holder, __NETIO_MAKE_FILELINE(), inactivity_type::start_read_body);
        }

        char *sbuf = m_body_buf.get();
        std::size_t to_read = m_header;
        m_sock->async_read_some(
             sbuf
            ,to_read
            ,[this, sbuf, to_read]
             (impl_holder holder, const char *fl, int ec, const char *emsg, char *buf, std::size_t size)
             { body_readed(std::move(holder), fl, ec, emsg, sbuf, buf, to_read, size); }
            ,std::move(holder)
        );
    }

    void body_readed(impl_holder holder, const char *fl, int ec, const char *emsg, char *sbuf, char *buf, std::size_t to_read, std::size_t readed) {
        (void)fl;

        if (m_read_some_handler) {
            __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                m_read_some_handler(
                     holder
                    ,__NETIO_MAKE_FILELINE()
                    ,ec
                    ,emsg
                    ,buf
                    ,readed
                );
            )
        }

        if ( !ec && readed != to_read ) {
//            std::cout << "to_read=" << to_read << ", readed=" << readed << std::endl;

            to_read = to_read-readed;
            if ( to_read ) {
                m_sock->async_read_some(
                     buf+readed
                    ,to_read
                    ,[this, sbuf, to_read]
                     (impl_holder holder, const char *fl, int ec, const char *emsg, char *buf, std::size_t size)
                     { body_readed(std::move(holder), fl, ec, emsg, sbuf, buf, to_read, size); }
                    ,std::move(holder)
                );

                return;
            }
        }

#ifdef NETIO_DEBUG
        std::cout << "m_netio::body_readed(): ec=" << ec << std::endl;
#endif // NETIO_DEBUG

        if ( ec ) {
            stop_services();
            m_handshake_finished = false;
            close_or_realloc_socket();

            if ( m_flags & test_inactivity ) {
                m_inactivity_timer->stop();
            }

            if ( !m_stop_requested ) {
                if ( m_error_handler ) {
                    __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                        m_error_handler(
                             std::move(holder)
                            ,__NETIO_MAKE_FILELINE()
                            ,ec
                            ,emsg
                        );
                    )
                }
            } else {
                m_stop_requested = false;
                if ( m_netio_stop_handler ) {
                    __NETIO_CATCH_AND_REPORT_EXCEPTIONS(m_netio_stop_handler(std::move(holder));)
                }
            }

            return;
        }

        if ( m_flags & test_inactivity ) {
            inactivity_timer_restart(holder, __NETIO_MAKE_FILELINE(), inactivity_type::body_readed);
        }

        if ( m_read_handler ) {
            __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                m_read_handler(
                     holder
                    ,__NETIO_MAKE_FILELINE()
                    ,ec
                    ,emsg
                    ,sbuf
                    ,m_header
                );
            )
        }

        std::shared_ptr<char> unpacked_buf{};
        std::size_t unpacked_size{};

        if ( is_no_crypt() ) {
            // тут deleter должен быть пустым, потому что'buf' хранится в 'm_body_buf'.
            unpacked_buf.reset(sbuf, [](char *){});
            unpacked_size = m_header;
        } else {
            try {
                m_crypto.unpack_packet(&unpacked_buf, &unpacked_size, sbuf, m_header);
            } catch (const std::exception &ex) {
                stop_services();
                m_handshake_finished = false;
                close_or_realloc_socket();

                std::string emsg = "unpack_packet(): std::exception was thrown: ";
                emsg += ex.what();
                if (m_error_handler) {
                    __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                        m_error_handler(
                             std::move(holder)
                            ,__NETIO_MAKE_FILELINE()
                            ,-1
                            ,emsg.c_str()
                        );
                    )
                }

                return;
            } catch (...) {
                stop_services();
                m_handshake_finished = false;
                close_or_realloc_socket();

                if (m_error_handler) {
                    __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                        m_error_handler(
                             std::move(holder)
                            ,__NETIO_MAKE_FILELINE()
                            ,-1
                            ,"unpack_packet(): unknown exception was thrown"
                        );
                    )
                }

                return;
            }
        }

#ifdef NETIO_DEBUG
        std::cout << "m_netio::body_readed(): unpacket=" << unpacked_size << ", packet=" << size << std::endl;
#endif // NETIO_DEBUG

        yas::mem_istream is(unpacked_buf.get(), unpacked_size);
        std::string clname, obname;
        try {
            std::uint8_t cllen{}, oblen{};

            yas::binary_iarchive<yas::mem_istream> ia(is);

            ia.read(cllen);
            clname.resize(cllen);
            ia.read(&clname[0], cllen);

            ia.read(oblen);
            obname.resize(oblen);
            ia.read(&obname[0], oblen);
        } catch (const std::exception &ex) {
            stop_services();
            m_handshake_finished = false;
            close_or_realloc_socket();

            std::string emsg = "when deserializing packet: std::exception was thrown: ";
            emsg += ex.what();
            if ( m_error_handler ) {
                __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                    m_error_handler(
                         std::move(holder)
                        ,__NETIO_MAKE_FILELINE()
                        ,-1
                        ,emsg.c_str()
                    );
                )
            }

            return;
        } catch (...) {
            stop_services();
            m_handshake_finished = false;
            close_or_realloc_socket();

            if ( m_error_handler ) {
                __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                    m_error_handler(
                         std::move(holder)
                        ,__NETIO_MAKE_FILELINE()
                        ,-1
                        ,"when deserializing packet: unknown exception was thrown"
                    );
                )
            }

            return;
        }

        const yas::intrusive_buffer ibuf = is.get_intrusive_buffer();

        if ( m_incoming_handler ) {
            __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                m_incoming_handler(
                     holder
                    ,clname.c_str()
                    ,obname.c_str()
                    ,ibuf.data
                    ,ibuf.size
                );
            )
        }

        try {
            m_distributor.invoke(clname, obname, ibuf.data, ibuf.size);
        } catch (const std::exception &ex) {
            stop_services();
            m_handshake_finished = false;
            close_or_realloc_socket();

            std::string emsg = "when invoking("+clname+"."+obname+"): std::exception was thrown: ";
            emsg += ex.what();
            if ( m_error_handler ) {
                __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                    m_error_handler(
                         std::move(holder)
                        ,__NETIO_MAKE_FILELINE()
                        ,-1
                        ,emsg.c_str()
                    );
                )
            }

            return;
        } catch (...) {
            stop_services();
            m_handshake_finished = false;
            close_or_realloc_socket();

            if ( m_error_handler ) {
                std::string emsg = "when invoking("+clname+"."+obname+"): unknown exception was thrown";
                __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                    m_error_handler(
                         std::move(holder)
                        ,__NETIO_MAKE_FILELINE()
                        ,-1
                        ,emsg.c_str()
                    );
                )
            }

            return;
        }

        read_header(std::move(holder));
    }

    void send(impl_holder holder, yas::shared_buffer ybuf, on_write_handler wh) {
        if ( !m_write_in_process ) {
            m_write_in_process = true;

            std::size_t to_write = ybuf.size;
            m_sock->async_write_some(
                 ybuf.data.get()
                ,to_write
                ,[this, to_write, ybuf, wh]
                 (impl_holder holder, const char *fl, int ec, const char *emsg, const char *buf, std::size_t writen)
                 { sent(fl, ec, emsg, buf, to_write, writen, std::move(holder), std::move(ybuf), std::move(wh)); }
                ,std::move(holder)
            );
        } else {
            m_buffers.push_back({std::move(holder), std::move(ybuf), std::move(wh)});
        }
    }

    void sent(
         const char *fl, int ec, const char *emsg, const char *buf, std::size_t to_write
        ,std::size_t writen, impl_holder holder, yas::shared_buffer ybuf, on_write_handler wh)
    {
        (void)fl;

        if ( m_write_some_handler ) {
            __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                m_write_some_handler(
                    holder
                    ,__NETIO_MAKE_FILELINE()
                    ,ec
                    ,emsg
                    ,buf
                    ,writen
                );
            )
        }

        if ( !ec && writen != to_write ) {
//            std::cout << "to_write=" << to_write << ", writen=" << writen << std::endl;

            to_write = to_write-writen;
            if ( to_write ) {
                m_sock->async_write_some(
                     buf+writen
                    ,to_write
                    ,[this, to_write, ybuf, wh]
                     (impl_holder holder, const char *fl, int ec, const char *emsg, const char *buf, std::size_t writen)
                     { sent(fl, ec, emsg, buf, to_write, writen, std::move(holder), std::move(ybuf), std::move(wh)); }
                    ,std::move(holder)
                );

                return;
            }
        }

        m_write_in_process = false;

        if ( ec ) {
            stop_services();
            m_handshake_finished = false;
            close_or_realloc_socket();

            m_buffers.clear();

            if ( m_error_handler ) {
                __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                    m_error_handler(
                         std::move(holder)
                        ,__NETIO_MAKE_FILELINE()
                        ,ec
                        ,emsg
                    );
                )
            }

            return;
        }

        if ( wh ) {
            __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                wh(
                     holder
                    ,__NETIO_MAKE_FILELINE()
                    ,ec
                    ,emsg
                    ,ybuf.data.get()
                    ,ybuf.size
                );
            )
        }
        if ( m_write_handler ) {
            __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                m_write_handler(
                     holder
                    ,__NETIO_MAKE_FILELINE()
                    ,ec
                    ,emsg
                    ,ybuf.data.get()
                    ,ybuf.size
                );
            )
        }

        if ( !m_buffers.empty() ) {
            queue_item item = std::move(m_buffers.front());
            m_buffers.pop_front();

            send(std::move(item.m_holder), std::move(item.m_buf), std::move(item.m_wrh));
        }
    }

    // services
    bool has_service(const char *rtti_name) const {
        auto it = std::find_if(
             m_serv.begin()
            ,m_serv.end()
            ,[rtti_name](const std::pair<std::string, std::shared_ptr<service_base>> &v)
             { return v.first == rtti_name; }
        );

        return it != m_serv.end();
    }

    service_handle get_service(const char *rtti_name) const {
        auto it = std::find_if(
             m_serv.begin()
            ,m_serv.end()
            ,[rtti_name](const std::pair<std::string, std::shared_ptr<service_base>> &v)
            { return v.first == rtti_name; }
        );
        if ( it == m_serv.end() ) {
            service_handle h{std::shared_ptr<service_base>()};
            return h;
        }

        service_handle h{it->second};
        return h;
    }
    service_handle add_service(const char *rtti_name, std::shared_ptr<service_base> b) {
        m_serv.push_back({rtti_name, b});

        service_handle h{b};
        return h;
    }
    void del_service(const char *rtti_name) {
        auto it = std::find_if(
             m_serv.begin()
            ,m_serv.end()
            ,[rtti_name](const std::pair<std::string, std::shared_ptr<service_base>> &v)
            { return v.first == rtti_name; }
        );

        if ( it == m_serv.end() )
            return;

        m_serv.erase(it);
    }

    worker &m_worker;
    socket_type m_sock;
    std::size_t m_flags;
    std::string m_ip;
    std::uint16_t m_port;
    std::size_t m_inactivity_timeout;
    timer_type m_inactivity_timer;
    std::size_t m_max_body_size;
    on_error_handler m_error_handler;
    on_read_handler m_read_handler;
    on_write_handler m_write_handler;
    on_read_handler m_read_some_handler;
    on_write_handler m_write_some_handler;
    on_incoming_handler m_incoming_handler;
    on_netio_stop_handler m_netio_stop_handler;
    distributor m_distributor;
    cryptotools m_crypto;
    cryptotools::pk_and_nonce_storage m_tmp_pk_and_nonce;
    bool m_handshake_finished;
    bool m_write_in_process;
    bool m_stop_requested;
    std::uint32_t m_header;
    std::unique_ptr<char[]> m_body_buf;
    std::size_t m_body_buf_size;

    struct queue_item {
        impl_holder m_holder;
        yas::shared_buffer m_buf;
        on_write_handler m_wrh;
    };
    std::deque<queue_item> m_buffers;

    // services
    std::vector<
         std::pair<std::string, std::shared_ptr<service_base>>
    > m_serv;
}; // struct impl

/**************************************************************************/

netio::netio(
     worker &w
    ,std::size_t flags
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
        ,flags
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

netio::netio(
     socket_type sock
    ,std::size_t flags
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
         std::move(sock)
        ,flags
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

netio::~netio()
{}

/**************************************************************************/

std::size_t netio::flags() const { return pimpl->flags(); }
bool netio::is_client_side() const { return pimpl->is_client_side(); }
bool netio::is_server_side() const { return pimpl->is_server_side(); }
bool netio::is_no_crypt() const { return pimpl->is_no_crypt();}

std::size_t netio::get_inactivity_timeout() const { return pimpl->m_inactivity_timeout; }
void netio::set_inactivity_timeout(std::size_t seconds) { pimpl->m_inactivity_timeout = seconds; }

std::size_t netio::get_max_body_size() const { return pimpl->m_max_body_size; }
void netio::set_max_body_size(std::size_t size) { pimpl->m_max_body_size = size; }

std::size_t netio::get_compress_if_greater() const { return pimpl->m_crypto.get_compress_if_greater(); }
void netio::set_compress_if_greater(std::size_t size) { pimpl->m_crypto.set_compress_if_greater(size); }

const on_error_handler& netio::get_error_handler() const { return pimpl->m_error_handler; }
void netio::set_error_handler(on_error_handler cb) { pimpl->m_error_handler = std::move(cb); }

const on_read_handler& netio::get_read_handler() const { return pimpl->m_read_handler; }
void netio::set_read_handler(on_read_handler cb) { pimpl->m_read_handler = std::move(cb); }

const on_write_handler& netio::get_write_handler() const { return pimpl->m_write_handler; }
void netio::set_write_handler(on_write_handler cb) { pimpl->m_write_handler = std::move(cb); }

const on_read_handler& netio::get_read_some_handler() const { return pimpl->m_read_some_handler; }
void netio::set_read_some_handler(on_read_handler cb) { pimpl->m_read_some_handler = std::move(cb); }

const on_write_handler& netio::get_write_some_handler() const { return pimpl->m_write_some_handler; }
void netio::set_write_some_handler(on_write_handler cb) { pimpl->m_write_some_handler = std::move(cb); }

const on_incoming_handler& netio::get_incoming_handler() const { return pimpl->m_incoming_handler; }
void netio::set_incoming_handler(on_incoming_handler cb) { pimpl->m_incoming_handler = std::move(cb); }

const on_netio_stop_handler& netio::get_netio_stop_handler() const { return pimpl->m_netio_stop_handler; }
void netio::set_netio_stop_handler(on_netio_stop_handler cb) { pimpl->m_netio_stop_handler = std::move(cb); }

/**************************************************************************/

void netio::connect(const char *ip, std::uint16_t port, int *ec, std::string *emsg) {
    pimpl->m_sock->connect(ip, port, ec, emsg);
}

void netio::async_connect(const char *ip, std::uint16_t port, on_connect_handler cb, impl_holder holder) {
    pimpl->async_connect(timer_type(), ip, port, std::move(cb), std::move(holder));
}

/**************************************************************************/

void netio::handshake(int *ec, std::string *emsg) {
    if ( pimpl->m_flags & no_crypt ) {
        if ( ec ) *ec = -1;
        if ( emsg ) *emsg = "handshake are not allowed when \"netio::no_crypt\" is specified";

        return;
    }

    (pimpl->m_flags & client)
        ? pimpl->handshake_as_client(ec, emsg)
        : pimpl->handshake_as_server(ec, emsg)
    ;
}

void netio::async_handshake(on_handshake_handler cb, impl_holder holder) {
    if ( pimpl->m_flags & no_crypt ) {
        if ( cb ) {
            __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                cb(
                     std::move(holder)
                    ,__NETIO_MAKE_FILELINE()
                    ,-1
                    ,"handshake are not allowed when \"netio::no_crypt\" is specified"
                );
            )
        }

        return;
    }

    (pimpl->m_flags & client)
        ? pimpl->async_handshake_as_client(std::move(cb), std::move(holder))
        : pimpl->async_handshake_as_server(std::move(cb), std::move(holder))
    ;
}

/**************************************************************************/

const std::uint8_t* netio::shared_key() const { return pimpl->m_crypto.get_shared_key(); }
std::size_t netio::shared_key_size() const { return pimpl->m_crypto.shared_key_size(); }

/**************************************************************************/

socket_type netio::get_socket() { return pimpl->m_sock; }
worker& netio::get_worker() { return pimpl->m_worker; }

const std::string& netio::get_ip() const { return pimpl->m_ip; }
std::uint16_t netio::get_port() const { return pimpl->m_port; }

/**************************************************************************/

void netio::start(impl_holder holder) { pimpl->start(std::move(holder)); }
void netio::stop() { pimpl->stop(); }

bool netio::started() const {
    return (
        is_no_crypt()
            ? pimpl->m_sock->is_open()
            : (pimpl->m_handshake_finished && pimpl->m_sock->is_open())
    );
}

/**************************************************************************/

object_registry_ptr netio::create_registry(const std::string &object) {
    return pimpl->m_distributor.create_registry(object);
}

/**************************************************************************/

yas::shared_buffer
netio::_prepare_packet(const char *api, std::size_t apilen, yas::shared_buffer buf) {
    const char *p = std::strrchr(api, '.');

    yas::mem_ostream os;
    yas::binary_oarchive<yas::mem_ostream> oa(os);

    const std::uint8_t olen = static_cast<std::uint8_t>(p - api);
    const std::uint8_t flen = static_cast<std::uint8_t>(api + apilen - p - 1);

    oa.write(olen);
    oa.write(api, olen);

    oa.write(flen);
    oa.write(p + 1, flen);

    if ( buf.size ) {
       oa.write(buf.data.get(), buf.size);
    }

    return os.get_shared_buffer();
}

yas::shared_buffer
netio::_prepare_packet(const char *obj, std::size_t objlen, const char *func, std::size_t funclen, yas::shared_buffer buf) {
    yas::mem_ostream os;
    yas::binary_oarchive<yas::mem_ostream> oa(os);

    const std::uint8_t olen = static_cast<std::uint8_t>(objlen);
    const std::uint8_t flen = static_cast<std::uint8_t>(funclen);

    oa.write(olen);
    oa.write(obj, olen);

    oa.write(flen);
    oa.write(func, flen);

    if ( buf.size ) {
        oa.write(buf.data.get(), buf.size);
    }

    return os.get_shared_buffer();
}

void netio::send_packet(impl_holder holder, yas::shared_buffer buf) {
    if ( !started() ) {
        if ( pimpl->m_error_handler ) {
            __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                pimpl->m_error_handler(
                     std::move(holder)
                    ,__NETIO_MAKE_FILELINE()
                    ,-1
                    ,"netio not started"
                );
            )
        }

        stop();

        return;
    }

    if ( is_no_crypt() ) {
        std::shared_ptr<char> packet_buf(new char[buf.size+sizeof(std::uint32_t)], [](char *p){delete[]p;});

        *reinterpret_cast<std::uint32_t*>(packet_buf.get()) = static_cast<std::uint32_t>(buf.size);
        std::memcpy(packet_buf.get()+sizeof(std::uint32_t), buf.data.get(), buf.size);

        yas::shared_buffer ybuf;
        ybuf.data = std::move(packet_buf);
        ybuf.size = buf.size+sizeof(std::uint32_t);

        pimpl->send(std::move(holder), std::move(ybuf), on_write_handler());

        return;
    }

    std::shared_ptr<char> packet_buf{};
    std::size_t packet_size{};
    try {
        pimpl->m_crypto.pack_packet(&packet_buf, &packet_size, buf.data.get(), buf.size);
    } catch (const std::exception &ex) {
        std::string emsg = "when invoking: std::exception was thrown: ";
        emsg += ex.what();
        if ( pimpl->m_error_handler ) {
            __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                pimpl->m_error_handler(
                     std::move(holder)
                    ,__NETIO_MAKE_FILELINE()
                    ,-1
                    ,emsg.c_str()
                );
            )
        }

        return;
    } catch (...) {
        if ( pimpl->m_error_handler ) {
            __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                pimpl->m_error_handler(
                     std::move(holder)
                    ,__NETIO_MAKE_FILELINE()
                    ,-1
                    ,"when invoking: unknown exception was thrown"
                );
            )
        }

        return;
    }
#ifdef NETIO_DEBUG
    std::cout << "netio::send(): unpacket=" << buf.size << ", packet=" << packet_size << std::endl;
#endif // NETIO_DEBUG

    yas::shared_buffer ybuf{std::move(packet_buf), packet_size};
    pimpl->send(std::move(holder), std::move(ybuf), on_write_handler());
}

/**************************************************************************/

void netio::write_buffer(impl_holder holder, yas::shared_buffer buf, on_write_handler wh) {
    if ( !started() ) {
        if ( wh ) {
            __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                wh(
                    std::move(holder)
                    ,__NETIO_MAKE_FILELINE()
                    ,-1
                    ,"netio not started"
                    ,nullptr
                    ,0
                );
            )
        }

        stop();

        return;
    }

    pimpl->send(std::move(holder), std::move(buf), std::move(wh));
}

/**************************************************************************/

void netio::read_buffer(impl_holder holder, char *ptr, std::size_t size, on_read_handler rh) {
    if ( !started() ) {
        if ( rh ) {
            __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                rh(
                     std::move(holder)
                    ,__NETIO_MAKE_FILELINE()
                    ,-1
                    ,"netio not started"
                    ,nullptr
                    ,0
                );
            )
        }

        stop();

        return;
    }

    auto cb = [this, rh](impl_holder holder, const char *fl, int ec, const char *emsg, char *buf, std::size_t size) {
        if ( pimpl->m_read_handler ) {
            __NETIO_CATCH_AND_REPORT_EXCEPTIONS(
                pimpl->m_read_handler(
                     holder
                    ,__NETIO_MAKE_FILELINE()
                    ,ec
                    ,emsg
                    ,buf
                    ,size
                );
            )
        }

        rh(std::move(holder), fl, ec, emsg, buf, size);
    };

    pimpl->m_sock->async_read(
         ptr
        ,size
        ,std::move(cb)
        ,std::move(holder)
    );
}

/**************************************************************************/

void netio::get_profdata(const get_profdata_cb &f) const {
    pimpl->m_distributor.get_profdata(f);
}

/**************************************************************************/

bool netio::has_service(const char *rtti_name) const {
    return pimpl->has_service(rtti_name);
}

service_handle netio::get_service(const char *rtti_name) const {
    return pimpl->get_service(rtti_name);
}

service_handle netio::add_service(const char *rtti_name, std::shared_ptr<service_base> b) {
    return pimpl->add_service(rtti_name, std::move(b));
}

void netio::del_service(const char *rtti_name) {
    pimpl->del_service(rtti_name);
}
/**************************************************************************/

} // ns net
} // ns gbp
