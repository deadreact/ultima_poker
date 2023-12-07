#pragma once

#include "types.h"

namespace gbp
{
 namespace ns_api
 {
  namespace ns_statistic
  {
   namespace ns_request
   {

    struct stat_common {
     virtual ~stat_common() { }
    };

    struct stat_desktop
    {
     virtual ~stat_desktop() { }

     // общеигровая статистика по столам и турнирам
     virtual void games_statistic() = 0;
     // доп. статистика по турниру
     virtual void get_additional_tourn_stat(gbp_i64 tourn_id) = 0;
     // зареганые в турнире юзеры
     virtual void get_tourn_users_stat(gbp_i64 tourn_id) = 0;
    };

    struct stat_mobile 
    {
     virtual ~stat_mobile() { }

     // стата по кешам
     virtual void mobile_get_cashes_stat() = 0;

     // стата по SNG
     virtual void mobile_get_sng_stat() = 0;

     // стата по турнирам
     virtual void mobile_get_tourn_stat() = 0;

    };

   } // ns_request
  } // ns_statistic
 } // ns_api
} // namespace gbp