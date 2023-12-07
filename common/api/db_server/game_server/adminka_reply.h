#pragma once

#include "../../api_common.h"
#include "../../adminko/reply.h"
#include "../../adminko/types.h"
#include "../../api_special_offers.h"

namespace gbp
{
 namespace ns_db_server
 {
  namespace ns_reply
  {
   struct adminka
   {
    virtual ~adminka() { }

    GBP_DECLARE_ENUM(
     e_adminka
     , unsigned char
     , friend
     , (ok)
       (unknown_error)
       // неправильное время по utc на компе с админкой
       (tourn_wrong_utc_time)
       // вектор процетов выплат пуст
       (tourn_payments_empty)
       // вектор завершения турнирных раундов пуст
       (tourn_round_ends_empty)
       /**
        * неправильные выплаты
        * Размер вектора 0, либо он меньше, чем макс. игроков в турнире */
       (tourn_wrong_win_payments)
       // spin-sng с таким бай-ином уже запущен
       (tourn_spin_sng_already_exists)
       // неправильный ID spin-sng пула
       (tourn_wrong_spin_sng_pool_id)
       // список уровней пула sng слишком мал
       (tourn_too_little_spin_sng_levels)
       (qual_not_main)
    )

    virtual void login(
     gbp_i64 admin_id
     , gbp::ns_db_server::ns_reply::adminka::e_adminka result
     , const gbp::ns_api::ns_admin::admin_auth_info_t& admin_info) = 0;

    virtual void get_hardwares_list(
     gbp_i64 admin_id
     , gbp::ns_db_server::ns_reply::adminka::e_adminka result
     , const std::vector<ns_api::ns_admin::hardware_t>& list) = 0;

    virtual void block_user_hardware(
     gbp_i64 admin_id
     , gbp::ns_db_server::ns_reply::adminka::e_adminka result
     , gbp::ns_api::ns_admin::hardware_t::e_status status
     , const std::string& hardware_id) = 0;

    virtual void add_cash(
     gbp_i64 admin_id
     , gbp::ns_db_server::ns_reply::adminka::e_adminka result
     , const std::vector<gbp::ns_api::ns_common::game_settings>& game_settings_) = 0;

    virtual void add_tournament(
     gbp_i64 admin_id
     , gbp::ns_api::ns_common::e_common_result result
     , const gbp::ns_api::ns_common::tournament_template_t& templ
     , const std::vector<gbp::ns_api::ns_common::tournament_settings>& tourn_settings) = 0;

    virtual void get_payments_in_list(
     gbp_i64 admin_id
     , gbp::ns_db_server::ns_reply::adminka::e_adminka result
     , gbp_i64 total_count
     , const std::vector<ns_api::ns_admin::ns_payments::payment_in>& list) = 0;

    virtual void get_payments_out_list(
     gbp_i64 admin_id
     , gbp::ns_db_server::ns_reply::adminka::e_adminka result
     , gbp_i64 total_count
     , const std::vector<ns_api::payment_out>& list) = 0;

    virtual void get_money_transfers(
     gbp_i64 admin_id
     , gbp::ns_db_server::ns_reply::adminka::e_adminka result
     , gbp_i64 total_count
     , const std::vector<ns_api::ns_admin::money_transfer_item_t>& items) = 0;

    virtual void get_special_offer_times(
     gbp_i64 admin_id
     , gbp::ns_db_server::ns_reply::adminka::e_adminka result
     // first - времена старта, second - времена остановки
     , const std::pair<std::vector<ns_api::time_s>, std::vector<ns_api::time_s>>& pair_) = 0;

    // admins
    virtual void add_new_admin(
     gbp_i64 admin_id
     , gbp::ns_db_server::ns_reply::adminka::e_adminka result
     , const gbp::ns_api::ns_admin::new_admin_t& new_admin) = 0;

    virtual void get_admins(
     gbp_i64 admin_id
     , gbp::ns_db_server::ns_reply::adminka::e_adminka result
     , const std::vector<gbp::ns_api::ns_admin::admin_item_t>& admins) = 0;

   }; // adminka
  } // ns_reply
 } // ns_db_server
} // namespace gbp