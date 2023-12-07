#pragma once

#include "types.h"
#include "admin_reply.h"

namespace gbp
{
 namespace ns_api
 {
  namespace ns_bots
  {
   namespace ns_botserver_reply
   { // game_server -> bot_server

    /****************************** MISCELLANEOUS *******************************/

    struct miscellaneous
    {
     virtual ~miscellaneous() { }

     GBP_DECLARE_ENUM(
       e_result
       , gbp_u8
       , friend
       , (ok)
       (unknown_error)
      )

     virtual void get_chat_cashes(const std::unordered_map<gbp_i64, gbp::ns_api::ns_bots::chat_cash>& chat_cashes) = 0;
     virtual void get_single_cash_info(
      gbp::ns_api::ns_bots::ns_admin_reply::miscellaneous::e_result result
      , gbp_i64 admin_id
      , const gbp::ns_api::ns_bots::single_cash_info& cash_info) = 0;
     virtual void get_day_fin_plan(
      gbp::ns_api::ns_bots::ns_botserver_reply::miscellaneous::e_result result
      , gbp_i64 admin_id
      , const ns_api::fin_plan_day_item& fin_plan) = 0;
     // взять список стран
     virtual void get_countries_list(
      gbp::ns_api::ns_bots::ns_botserver_reply::miscellaneous::e_result result
      , gbp_i64 admin_id
      , const std::vector<gbp::ns_api::country_item>& countries) = 0;
     virtual void player_close_table(
      gbp::ns_api::ns_bots::ns_botserver_reply::miscellaneous::e_result result
      , gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id) = 0;

    };

    /******************************** CASHES ************************************/

    struct cashes
    {
     virtual ~cashes() { }

     virtual void get_cashes(const std::vector<gbp::ns_api::ns_bots::cash>& cashes) = 0;
    };

    /****************************** TOURNAMENTS *********************************/

    struct tournaments
    {
     virtual ~tournaments() { }

     virtual void get_tournaments(const std::vector<gbp::ns_api::ns_bots::tournament>& tournaments) = 0;
     
     GBP_DECLARE_ENUM(
       e_get_additional_tournament_stat
       , gbp_u8
       , friend
       , (ok)
       (unknown_error)
      )
     virtual void get_additional_tournament_stat(
      gbp::ns_api::ns_bots::ns_botserver_reply::tournaments::e_get_additional_tournament_stat result
      , gbp_i64 admin_id
      , const gbp::ns_api::ns_bots::additional_tournament_stat& tournament_stat) = 0;

    };
    
    /************************* PLANNED DISTRIBUTIONS ****************************/

    struct planned_distributions
    {
     virtual ~planned_distributions() { }

     GBP_DECLARE_ENUM(
       e_result
       , gbp_u8
       , friend
       , (ok)
       (unknown_error)
       (removing_not_available)
      )
     virtual void get_planned_distributions(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp::ns_api::ns_bots::ns_botserver_reply::planned_distributions::e_result result
      , const std::unordered_map<gbp_i64, ns_api::ns_bots::planned_distribution>& planned_distributions
      , const std::vector<gbp::ns_api::ns_bots::player>& players
      , gbp_i64 current_distribution) = 0;

     virtual void add_planned_distribution(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp::ns_api::ns_bots::ns_botserver_reply::planned_distributions::e_result result) = 0;

     virtual void remove_planned_distribution(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp::ns_api::ns_bots::ns_botserver_reply::planned_distributions::e_result result) = 0;
     
     virtual void get_planned_distr_history_items(
      gbp::ns_api::ns_bots::ns_botserver_reply::planned_distributions::e_result result
      , gbp_i64 admin_id
      , gbp_i64 tournament_id
      , bool is_sit_n_go
      , gbp_i64 game_id
      , const std::vector<gbp::ns_api::ns_bots::planned_distr_templ_history_item>& items) = 0;

     virtual void remove_planned_distr_history_item(
      gbp::ns_api::ns_bots::ns_botserver_reply::planned_distributions::e_result result
      , gbp_i64 admin_id
      , gbp_i64 tournament_id
      , bool is_sit_n_go
      , gbp_i64 game_id) = 0;

    };

    /********************************** CHASE **********************************/

    struct chase
    {
     virtual ~chase() { }

     virtual void get_full_chase_stat(
      const std::unordered_map<gbp_i64, ns_api::ns_bots::chase_cash>& chase_cashes
      , const std::unordered_map<gbp_i64, ns_api::ns_bots::chase_tournament_cash>& chase_tournament_cashes) = 0;

     // ответ игрового на обновление заметки по юзеру
     virtual void update_user_notes_result(gbp::ns_api::ns_bots::ns_admin_reply::chase::e_chase result, gbp_i64 admin_id, gbp_i64 user_id) = 0;

     virtual void update_user_task(
      gbp::ns_api::ns_bots::ns_admin_reply::chase::e_chase result
      , gbp_i64 admin_id
      , gbp_i64 user_id) = 0;

     virtual void get_tournament_prizes(gbp_i64 tournament_id, gbp_i64 admin_id, bool is_sit_n_go, const std::vector<ns_api::money>& prizes) = 0;

    };

   } // ns ns_botserver_reply
  } // ns ns_bots
 } // ns ns_api
} // ns gbp