
#ifndef __gbp__net__services__remote_time_service_hpp
#define __gbp__net__services__remote_time_service_hpp

#include <netio/netio.hpp>

namespace gbp {
namespace net {

/*********************************************************************************************************************/

struct remote_time_service: gbp::net::service_base {
    remote_time_service(gbp::net::netio &netio);
    virtual ~remote_time_service();

    bool start(gbp::net::impl_holder holder = gbp::net::impl_holder()) override;
    void stop() override;

    bool started() const override;

    void get_time(std::function<void(std::uint64_t)> cb, gbp::net::impl_holder holder = gbp::net::impl_holder());

private:
    struct impl;
    impl *pimpl;
};

/*********************************************************************************************************************/

} // ns net
} // ns gbp

#endif // __gbp__net__services__remote_time_service_hpp
