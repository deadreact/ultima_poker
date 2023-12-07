
#include <netio/worker.hpp>
#include <netio/timer.hpp>

#include <iostream>

/*************************************************************************************************/

int main(int, char **) {
    gbp::net::worker worker;

    gbp::net::impl_holder holder0 = gbp::net::impl_holder();
    gbp::net::timer_type t0 = gbp::net::create_one_shot_timer(worker);
    t0->start(
         333
        ,[](gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg) {
            (void)holder;

            std::cout << "one shot timer handler: fl=" << fl << ", ec=" << ec << ", emsg=" << emsg << std::endl;
        }
        ,std::move(holder0)
    );

    gbp::net::impl_holder holder1 = gbp::net::impl_holder();
    gbp::net::timer_type t1 = gbp::net::create_periodic_timer(worker);
    t1->start(
         555
        ,[](gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg) {
            (void)holder;

            std::cout << "periodic timer handler: fl=" << fl << ", ec=" << ec << ", emsg=" << emsg << std::endl;
        }
        ,std::move(holder1)
    );

    worker.run();
}

/*************************************************************************************************/
