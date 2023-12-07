
#ifndef __gbp__net__services__disconnect_service_hpp
#define __gbp__net__services__disconnect_service_hpp

#include <netio/netio.hpp>

namespace gbp {
namespace net {

/*********************************************************************************************************************/

struct disconnect_service: gbp::net::service_base {
    disconnect_service(gbp::net::netio &netio);
    virtual ~disconnect_service();

    bool start(gbp::net::impl_holder holder = gbp::net::impl_holder()) override;
    void stop() override;

    bool started() const override;

    void set_disconnect_handler(std::function<void(std::int32_t ec)> cb);
    void disconnect(std::int32_t ec = 0, gbp::net::impl_holder holder = gbp::net::impl_holder());

private:
    struct impl;
    impl *pimpl;
};

/*********************************************************************************************************************/

} // ns net
} // ns gbp

#endif // __gbp__net__services__disconnect_service_hpp
