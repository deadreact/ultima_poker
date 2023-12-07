#pragma once

namespace gbp {
namespace ns_api {
namespace ns_statistic {
namespace ns_request {
/******************* request ***********************/
struct stat_common;
struct stat_desktop;
/**************************************************/
}
namespace ns_reply {
/******************** reply ***********************/
struct stat_common;
struct stat_desktop;
/**************************************************/
}
}
}
}
/******************* emitter **********************/
#include "emitter_statistic.h"
/**************************************************/


//! @namespace ns_statistic
namespace ns_statistic
{
    struct rpc_object
    {
    public:
        virtual gbp::ns_api::ns_statistic::ns_request::stat_common*  request_common() const = 0;
        virtual gbp::ns_api::ns_statistic::ns_request::stat_desktop*  request_desktop() const = 0;
        virtual const emitter_t* emitter() const = 0;

        static QString classname_common();
        static QString classname_desktop();
    public:
        virtual void games_statistic(on_games_statistic callback = on_games_statistic());
        virtual void get_additional_tourn_stat(gbp_i64 tourn_id, on_get_additional_tourn_stat callback = on_get_additional_tourn_stat());
        virtual void get_tourn_users_stat(gbp_i64 tourn_id, on_get_tourn_users_stat<tournament_stat_t::user_info> callback = on_get_tourn_users_stat<tournament_stat_t::user_info>());

    };
} //namespace ns_statistic
