
#ifndef __gbp__net__timer_hpp
#define __gbp__net__timer_hpp

#include <netio/typedefs.hpp>

#include <memory>

namespace gbp {
namespace net {

struct worker;

/***************************************************************************/

struct i_timer {
    virtual ~i_timer() {}

    virtual worker& get_worker() = 0;

    virtual void start(std::size_t ms, on_timeout_handler cb, impl_holder holder = impl_holder()) = 0;
    virtual void stop() = 0;

    virtual bool started() const = 0;
};

using timer_type = std::shared_ptr<i_timer>;

timer_type create_one_shot_timer(worker &w);
timer_type create_periodic_timer(worker &w);

/***************************************************************************/

} // ns net
} // ns gbp

#endif // __gbp__net__timer_hpp
