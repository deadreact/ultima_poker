
#ifndef __gbp__net__service_handle_hpp
#define __gbp__net__service_handle_hpp

#include <netio/typedefs.hpp>

namespace gbp {
namespace net {

struct service_base;

/**************************************************************************/

struct service_handle {
    service_handle() = delete;

    service_handle(std::weak_ptr<service_base> b);

    service_handle(const service_handle &r);
    service_handle& operator= (const service_handle &r);

    service_handle(service_handle &&r);
    service_handle& operator= (service_handle &&r);

    virtual ~service_handle();

    template<typename T>
    T* get_service() {
        auto w = get_ptr();
        auto p = w.lock();
        if ( p ) {
            return static_cast<T *>(p.get());
        }

        return nullptr;
    }

    operator bool() const;

    bool start(impl_holder holder = impl_holder());
    void stop();

    bool started() const;

private:
    struct impl;
    impl *pimpl;

private:
    std::weak_ptr<service_base> get_ptr();
    const std::weak_ptr<service_base> get_ptr() const;
};

/**************************************************************************/

} // ns net
} // ns gbp

#endif // __gbp__net__service_handle_hpp
