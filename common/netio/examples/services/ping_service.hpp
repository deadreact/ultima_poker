
#ifndef __gbp__net__services__ping_service_hpp
#define __gbp__net__services__ping_service_hpp

#include <netio/netio.hpp>

namespace gbp {
namespace net {

/*********************************************************************************************************************/

struct ping_service: gbp::net::service_base {
    ping_service(
         gbp::net::netio &netio
        ,std::size_t interval
        ,std::function<void(std::uint64_t time_us)>
    );
    virtual ~ping_service();

    bool start(gbp::net::impl_holder holder = gbp::net::impl_holder()) override;
    void stop() override;

    bool started() const override;

private:
    struct impl;
    impl *pimpl;
};

/*********************************************************************************************************************/

} // ns net
} // ns gbp

#endif // __gbp__net__services__ping_service_hpp
