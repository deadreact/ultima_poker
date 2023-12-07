
#include <netio/timer.hpp>

#include <QCoreApplication>

#include <iostream>

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    gbp::net::timer_type t0 = gbp::net::create_one_shot_timer(&app);
    gbp::net::impl_holder fake_holder{};
    t0->start(
         3333
        ,[](gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg) {
            (void)holder;

            std::cout << "one shot timer handler: fl=" << fl << ", ec=" << ec << ", emsg=" << emsg << std::endl;
        }
        ,std::move(fake_holder)
    );

//    gbp::net::timer_type t1 = gbp::net::create_periodic_timer(&app);
//    t1->start(
//         555
//        ,[](gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg) {
//            (void)holder;
//            std::cout << "periodic timer handler: fl=" << fl << ", ec=" << ec << ", emsg=" << emsg << std::endl;
//        }
//    );

    return app.exec();
}
