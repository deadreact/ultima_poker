#pragma once

#include "../../api_common.h"
#include "../../api_special_offers.h"
#include "../../adminko/types.h"

namespace gbp
{
 namespace ns_db_server
 {
  namespace ns_reply
  {
   struct game_controller
   {
    virtual ~game_controller() { }

    GBP_DECLARE_ENUM(
     e_game_controller
     , gbp_u8
     , friend
     , (ok)
       (unknown_error)
    )

    virtual void register_user(
     gbp::ns_db_server::ns_reply::game_controller::e_game_controller result
     , const gbp::ns_api::user_register_info_t& ui
     , const gbp::ns_api::ns_common::reg_new_user_db_result& reg_db_result) = 0;

    virtual void add_tournament(
     gbp::ns_db_server::ns_reply::game_controller::e_game_controller result
     , const gbp::ns_api::ns_common::add_new_tournament_result& db_result) = 0;
    virtual void get_tournaments_finished(
      gbp::ns_db_server::ns_reply::game_controller::e_game_controller result
      , gbp_i64 admin_id
      , const std::vector<gbp::ns_api::ns_admin::tourn_finished_item>& items
      , gbp_i32 all_items_count) = 0;
    virtual void get_finished_tourn_info(
      gbp::ns_db_server::ns_reply::game_controller::e_game_controller result
      , gbp_i64 admin_id
      , const gbp::ns_api::ns_admin::tourn_finished_info_t& info) = 0;
    virtual void get_cash_next_ids(
     gbp::ns_db_server::ns_reply::game_controller::e_game_controller result
     , const gbp::ns_api::ns_common::get_cash_next_ids_reply_type& rep_type) = 0;
    virtual void insert_admin_template(
     gbp::ns_db_server::ns_reply::game_controller::e_game_controller result
     , const gbp::ns_api::ns_common::admin_template& templ) = 0;
    virtual void add_sit_n_go_restore_template(
     gbp::ns_db_server::ns_reply::game_controller::e_game_controller result
     , gbp_i64 admin_id
     , gbp_i64 template_id
     , const gbp::ns_api::ns_common::sit_n_go_template_restore& item) = 0;
    virtual void get_countries(
     gbp::ns_db_server::ns_reply::game_controller::e_game_controller result
     , const std::map<gbp_i32, ns_api::country_item>& countries) = 0;
    virtual void affiliates_add(
     gbp::ns_db_server::ns_reply::game_controller::e_game_controller result
     , gbp_i64 admin_id
     , const gbp::ns_api::affiliate_item_t& item) = 0;
    virtual void affiliates_get_list(
     gbp::ns_db_server::ns_reply::game_controller::e_game_controller result
     , const std::vector<gbp::ns_api::affiliate_item_t>& affiliates) = 0;
    virtual void get_users(
     gbp::ns_db_server::ns_reply::game_controller::e_game_controller result
     , const std::vector<ns_api::ns_common::user_info>& users) = 0;
    virtual void get_cashes(
     gbp::ns_db_server::ns_reply::game_controller::e_game_controller result
     , const std::vector<gbp::ns_api::ns_common::game_settings>& cashes) = 0;
    virtual void get_tournaments(
     gbp::ns_db_server::ns_reply::game_controller::e_game_controller result
     , const std::unordered_map<gbp_i64, gbp::ns_api::ns_common::tournament_settings>& tournaments) = 0;
    virtual void get_sit_n_go_restore_templates(
     gbp::ns_db_server::ns_reply::game_controller::e_game_controller result
     , const std::unordered_map<gbp_i64, gbp::ns_api::ns_common::sit_n_go_template_restore>& templates) = 0;
    virtual void get_emails(
     gbp::ns_db_server::ns_reply::game_controller::e_game_controller result
     , const std::vector<ns_api::ns_common::email_info>& mails_info) = 0;
    virtual void get_admin_templates(
     gbp::ns_db_server::ns_reply::game_controller::e_game_controller result
     , const std::unordered_map<gbp_i64, gbp::ns_api::ns_common::admin_template>& admin_templates) = 0;
    virtual void get_common_statistic(
     gbp::ns_db_server::ns_reply::game_controller::e_game_controller result
     , const std::unordered_map<gbp_i64, gbp::ns_api::ns_common::stat_item>& statistic) = 0;
    virtual void get_special_offers(
     gbp::ns_db_server::ns_reply::game_controller::e_game_controller result
     , const std::vector<ns_api::ns_special_offers::special_offer>& special_offers) = 0;
    virtual void get_autocontrol_financial_plans(
     gbp::ns_db_server::ns_reply::game_controller::e_game_controller result
     , const std::unordered_map<gbp_i32, gbp::ns_api::fin_plan_day_item>& fin_days) = 0;
    virtual void get_vip_system(
     gbp::ns_db_server::ns_reply::game_controller::e_game_controller result
     , const ns_vip_system::load_info& info) = 0;
    virtual void add_vip_bonus(
     gbp::ns_db_server::ns_reply::game_controller::e_game_controller result
     // с обновлённым ID
     , const ns_vip_system::bonus_item& b_item) = 0;

   }; // game_controller
  } // ns_replies
 } // ns_db_server
} // namespace gbp