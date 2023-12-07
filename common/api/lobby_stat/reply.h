#pragma once

#include "types.h"

namespace gbp
{
 namespace ns_api
 {
  namespace ns_statistic
  {
   namespace ns_reply
   {

    GBP_DECLARE_ENUM(
      e_stat
      , gbp_u8
      , inline
      , (ok)
      (unknown_error)
     )

    struct stat_common {
     virtual ~stat_common() { }
    };

    struct stat_desktop
    {
     virtual ~stat_desktop() { }

     virtual void games_statistic(
      const std::pair<std::vector<gbp::ns_api::ns_statistic::cash_stat_t>, std::vector<gbp::ns_api::ns_statistic::tournament_stat_t>>& stat
     ) = 0;

     virtual void get_additional_tourn_stat(
      gbp::ns_api::ns_statistic::ns_reply::e_stat result
      , const gbp::ns_api::ns_statistic::tourn_additional_stat_t& stat) = 0;

     virtual void get_tourn_users_stat(
      gbp::ns_api::ns_statistic::ns_reply::e_stat result
      , gbp_i64 tourn_id
      , const std::vector<gbp::ns_api::ns_statistic::tournament_stat_t::user_info>& users_stat) = 0;

    };

    struct stat_mobile
    {
     virtual ~stat_mobile() { }

     virtual void mobile_get_cashes_stat(
      gbp::ns_api::ns_statistic::ns_reply::e_stat result
      , const std::vector<gbp::ns_api::ns_statistic::ns_mobile::item_cash_game>& items) = 0;

     virtual void mobile_get_sng_stat(
      gbp::ns_api::ns_statistic::ns_reply::e_stat result
      , const std::vector<gbp::ns_api::ns_statistic::ns_mobile::item_sng>& items) = 0;

     virtual void mobile_get_tourn_stat(
      gbp::ns_api::ns_statistic::ns_reply::e_stat result
      , const std::vector<gbp::ns_api::ns_statistic::ns_mobile::item_tourn>& items) = 0;

    };

   } // ns_reply
  } // ns_statistic
 } // ns_api
} // namespace gbp