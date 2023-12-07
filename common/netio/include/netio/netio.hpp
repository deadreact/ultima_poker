
#ifndef __gbo__net__netio_hpp
#define __gbo__net__netio_hpp

#include <netio/worker.hpp>
#include <netio/socket.hpp>
#include <netio/acceptor.hpp>
#include <netio/timer.hpp>
#include <netio/profdata.hpp>
#include <netio/cryptotools.hpp>
#include <netio/object_registry.hpp>
#include <netio/service_base.hpp>
#include <netio/service_handle.hpp>

#include <yas/mem_streams.hpp>
#include <yas/binary_oarchive.hpp>
#include <yas/serializers/serializer.hpp>
#include <yas/serializers/std_types_serializers.hpp>

#include <cstdint>
#include <string>
#include <functional>
#include <memory>
#include <typeinfo>

#ifdef _WIN32
#   include <winerror.h>
#   define __NETIO_SOCKET_ERROR(x) WSA ## x
#   define __NETIO_WIN_OR_POSIX(w, p) w
#else
#   include <cerrno>
#   define __NETIO_SOCKET_ERROR(x) x
#   define __NETIO_WIN_OR_POSIX(w, p) p
#endif // _WIN32

namespace gbp {
namespace net {

/**************************************************************************/

enum error {
     eof = 2
    ,connection_refused = __NETIO_SOCKET_ERROR(ECONNREFUSED)
    ,timed_out          = __NETIO_SOCKET_ERROR(ETIMEDOUT)
    ,bad_descriptor     = __NETIO_SOCKET_ERROR(EBADF)
    ,operation_aborted  = __NETIO_WIN_OR_POSIX(ERROR_OPERATION_ABORTED, ECANCELED)
};

/**************************************************************************/

struct netio {
    enum network_side {
         client          = 1u<<0
        ,server          = 1u<<1
        ,no_crypt        = 1u<<2
        ,test_inactivity = 1u<<3
    };

    enum: std::size_t {
         k_inactivity_timeout = 10 // secs
        ,k_max_body_size = 1024*1024*10 // 10 Mb
        ,k_compress_if_greater = 200
    };

    netio() = delete;
    netio(const netio &) = delete;
    netio& operator= (const netio &) = delete;
    netio& operator= (netio &&) = delete;

    netio(
         worker &w
        ,std::size_t flags
        ,std::size_t inactivity_timeout = k_inactivity_timeout
        ,std::size_t max_body_size = k_max_body_size
        ,std::size_t compress_if_greater = k_compress_if_greater
        ,on_error_handler eh = on_error_handler()
        ,on_read_handler rh = on_read_handler()
        ,on_write_handler wh = on_write_handler()
        ,on_incoming_handler ih = on_incoming_handler()
        ,on_netio_stop_handler sh = on_netio_stop_handler()
    );
    netio(
         socket_type sock
        ,std::size_t flags
        ,std::size_t inactivity_timeout = k_inactivity_timeout
        ,std::size_t max_body_size = k_max_body_size
        ,std::size_t compress_if_greater = k_compress_if_greater
        ,on_error_handler eh = on_error_handler()
        ,on_read_handler rh = on_read_handler()
        ,on_write_handler wh = on_write_handler()
        ,on_incoming_handler ih = on_incoming_handler()
        ,on_netio_stop_handler sh = on_netio_stop_handler()
    );
    netio(netio &&r) = default;
    virtual ~netio();

    // get flags
    std::size_t flags() const;
    bool is_client_side() const;
    bool is_server_side() const;
    bool is_no_crypt() const;

    // services
    template<typename T, typename... Args>
    service_handle add_service(Args &&... args) {
        const char *rtti_name = typeid(T).name();
        if ( has_service(rtti_name) ) {
            return get_service(rtti_name);
        }

        std::shared_ptr<service_base> p = std::make_shared<T>(*this, std::forward<Args>(args)...);
        return add_service(rtti_name, std::move(p));
    }
    template<typename T>
    T* get_service() {
        const char *rtti_name = typeid(T).name();
        if ( has_service(rtti_name) ) {
            auto h = get_service(rtti_name);
            return h.get_service<T>();
        }

        return nullptr;
    }
    template<typename T>
    void del_service() {
        const char *rtti_name = typeid(T).name();
        if ( has_service(rtti_name) ) {
            auto h = get_service(rtti_name);
            auto p = h.get_service<T>();
            assert(p != nullptr);
            if ( p->started() ) {
                p->stop();
            }
            del_service(rtti_name);
        }
    }

    /*  */
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

    const on_read_handler& get_read_some_handler() const;
    void set_read_some_handler(on_read_handler cb);

    const on_write_handler& get_write_some_handler() const;
    void set_write_some_handler(on_write_handler cb);

    const on_incoming_handler& get_incoming_handler() const;
    void set_incoming_handler(on_incoming_handler cb);

    const on_netio_stop_handler& get_netio_stop_handler() const;
    void set_netio_stop_handler(on_netio_stop_handler cb);

    /*  */
    void connect(const char *ip, std::uint16_t port, int *ec, std::string *emsg);
    void async_connect(const char *ip, std::uint16_t port, on_connect_handler cb, impl_holder holder = impl_holder());

    void handshake(int *ec, std::string *emsg);
    void async_handshake(on_handshake_handler hh, impl_holder holder = impl_holder());

    const std::uint8_t *shared_key() const;
    std::size_t shared_key_size() const;

    /*  */
    socket_type get_socket();
    worker& get_worker();

    /*  */
    const std::string& get_ip() const;
    std::uint16_t get_port() const;

    /*  */
    void start(impl_holder holder = impl_holder());
    void stop();

    // открыт ли сокет + завершен ли хэндшейк.
    bool started() const;

    /*  */
    object_registry_ptr create_registry(const std::string &object);

    /*************************************************************************/

    // сериализует в пакет имя API и аргументы(предварительно сериализованные
    //   при помощи 'serialize_args()'), полученные как 'buf'.
    // возвращает yas::shared_buffer.
    // прим: перегрузка для случаев, когда 'api' - указатель
    //   на 'const char*'. (не путать с "cstring"[N])
    template<typename ConstCharPtr, typename... Args>
    typename std::enable_if<
        std::is_same<ConstCharPtr, const char *>::value, yas::shared_buffer
    >::type
    prepare_packet(ConstCharPtr api, Args&&... args) {
        return _prepare_packet(api, std::strlen(api), serialize_args(std::forward<Args>(args)...));
    }

    // сериализует в пакет имя API и аргументы(предварительно сериализованные
    //   при помощи 'serialize_args()'), полученные как 'buf'.
    // возвращает yas::shared_buffer.
    // прим: перегрузка для случаев, когда 'api' - массив символов.
    //   (не путать с 'const char*')
    template<std::size_t N, typename... Args>
    yas::shared_buffer prepare_packet(const char(&api)[N], Args&&... args) {
        return _prepare_packet(api, N-1, serialize_args(std::forward<Args>(args)...));
    }

    // оптимизированная версия для случаев, когда имя объекта и функции
    // передается отдельными аргументами.
    // плюс в том, что нет необходимости в run-time искать символ точки.
    template<std::size_t NO, std::size_t NF, typename ...Args>
    yas::shared_buffer prepare_packet(const char (&obj)[NO], const char (&func)[NF], Args&& ...args) {
        return _prepare_packet(obj, NO-1, func, NF-1, serialize_args(std::forward<Args>(args)...));
    }

    /*************************************************************************/

    // вызов удаленной процедуры.
    // используется, как правило, на стороне сервера, потому как
    // первый аргумент - 'shared_ptr', который получен как результат
    // вызова 'shared_from_this()'.
    // прим: перегрузка для случаев, когда 'api' - указатель
    //   на 'const char*'. (не путать с "cstring"[N])
    template<typename ConstCharPtr, typename... Args>
    typename std::enable_if<
        std::is_same<ConstCharPtr, const char *>::value
    >::type
    call(impl_holder holder, ConstCharPtr api, Args&&... args) {
        yas::shared_buffer buf = _prepare_packet(api, std::strlen(api), serialize_args(std::forward<Args>(args)...));
        send_packet(std::move(holder), std::move(buf));
    }

    // вызов удаленной процедуры.
    // используется, как правило, на стороне сервера, потому как
    // первый аргумент - 'shared_ptr', который получен как результат
    // вызова 'shared_from_this()'.
    // прим: перегрузка для случаев, когда 'api' - массив символов.
    //   (не путать с 'const char*')
    template<std::size_t N, typename... Args>
    void call(impl_holder holder, const char(&api)[N], Args&&... args) {
        yas::shared_buffer buf = _prepare_packet(api, N-1, serialize_args(std::forward<Args>(args)...));
        send_packet(std::move(holder), std::move(buf));
    }

    // оптимизированная версия предыдущих двух функций для случаев, когда
    // имя объекта и функции передается отдельными аргументами.
    // плюс в том, что нет необходимости в run-time искать символ точки.
    template<std::size_t NO, std::size_t NF, typename ...Args>
    void call(impl_holder holder, const char (&obj)[NO], const char (&func)[NF], Args&& ...args) {
        yas::shared_buffer buf = _prepare_packet(obj, NO-1, func, NF-1, serialize_args(std::forward<Args>(args)...));
        send_packet(std::move(holder), std::move(buf));
    }

    /*************************************************************************/

    // вызов удаленной процедуры.
    // используется, как правило, на стороне клиента.
    // прим: перегрузка для случаев, когда 'api' - указатель
    //   на 'const char*'. (не путать с "cstring"[N])
    template<typename ConstCharPtr, typename... Args>
    typename std::enable_if<
        std::is_same<ConstCharPtr, const char *>::value
    >::type
    call2(ConstCharPtr api, Args&&... args) {
        yas::shared_buffer buf = _prepare_packet(api, std::strlen(api), serialize_args(std::forward<Args>(args)...));
        send_packet(impl_holder(), std::move(buf));
    }

    // вызов удаленной процедуры.
    // используется, как правило, на стороне клиента.
    // прим: перегрузка для случаев, когда 'api' - массив символов.
    //   (не путать с 'const char*')
    template<std::size_t N, typename... Args>
    void call2(const char(&api)[N], Args&&... args) {
        yas::shared_buffer buf = _prepare_packet(api, N-1, serialize_args(std::forward<Args>(args)...));
        send_packet(impl_holder(), std::move(buf));
    }

    // оптимизированная версия предыдущих двух функций для случаев, когда
    // имя объекта и функции передается отдельными аргументами.
    // плюс в том, что нет необходимости в run-time искать символ точки.
    template<std::size_t NO, std::size_t NF, typename ...Args>
    void call2(const char (&obj)[NO], const char (&func)[NF], Args&& ...args) {
        yas::shared_buffer buf = _prepare_packet(obj, NO-1, func, NF-1, serialize_args(std::forward<Args>(args)...));
        send_packet(impl_holder(), std::move(buf));
    }
    /*************************************************************************/

    // собственно, отправка данных противоположной стороне.
    // добавляет заголовок пакета, и сжимает/шифрует, если в конструкторе,
    // во флагах, не использовался 'no_crypt'.
    void send_packet(impl_holder holder, yas::shared_buffer buf);

    void send_packet2(yas::shared_buffer buf)
    { send_packet(impl_holder(), std::move(buf)); }

    /*************************************************************************/

    // записать данные в сокет.
    // от 'send_packet()/send_packet2()' двух отличается тем, что над буфером не производятся
    // никакие операции, т.е. записывается в сокет как есть.
    void write_buffer(impl_holder holder, yas::shared_buffer buf, on_write_handler wh = on_write_handler());

    void write_buffer2(yas::shared_buffer buf, on_write_handler wh = on_write_handler())
    { write_buffer(impl_holder(), std::move(buf), std::move(wh)); }

    // прочитать данные из сокета.
    // над буфером не производятся никакие операции, т.е. читается из сокета как есть.
    void read_buffer(impl_holder holder, char *ptr, std::size_t size, on_read_handler rh);

    void read_buffer2(char *ptr, std::size_t size, on_read_handler rh)
    { read_buffer(impl_holder(), ptr, size, std::move(rh)); }

    /*************************************************************************/

    void get_profdata(const get_profdata_cb &f) const;

private:
    template<typename... Args>
    yas::shared_buffer serialize_args(Args&&... args) {
        if (sizeof...(Args)) {
            yas::mem_ostream os;
            yas::binary_oarchive<yas::mem_ostream> oa(os, yas::no_header);
            oa & std::make_tuple(std::forward<Args>(args)...);
            return os.get_shared_buffer();
        }

        return yas::shared_buffer();
    }

    yas::shared_buffer _prepare_packet(const char *api, std::size_t apilen, yas::shared_buffer buf);
    yas::shared_buffer _prepare_packet(const char *obj, std::size_t objlen, const char *func, std::size_t funclen, yas::shared_buffer buf);

private:
    bool has_service(const char *rtti_name) const;
    service_handle add_service(const char *rtti_name, std::shared_ptr<service_base> b);
    service_handle get_service(const char *rtti_name) const;
    void del_service(const char *rtti_name);

private:
    struct impl;
    std::unique_ptr<impl> pimpl;
};

/**************************************************************************/

} // ns net
} // ns gbp

#endif // __gbo__net__netio_hpp
