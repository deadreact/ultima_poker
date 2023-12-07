#pragma once

#include "../api_common.h"
#include "types.h"

namespace gbp
{
 namespace ns_api
 {
  namespace ns_bots
  {
   namespace ns_admin_reply
   { // bot_server -> bot_adminka

    /***************************** MISCELLANEOUS ******************************/

    struct miscellaneous
    {
     virtual ~miscellaneous() { }

     GBP_DECLARE_ENUM(
      e_result
      , gbp_u8
      , friend
      , (ok)
      (unknown_error)
      (invalid_old_password)
     )

     virtual void change_own_password(gbp::ns_api::ns_bots::ns_admin_reply::miscellaneous::e_result result) = 0;

     virtual void get_single_cash_info(
      gbp::ns_api::ns_bots::ns_admin_reply::miscellaneous::e_result result
      , const gbp::ns_api::ns_bots::single_cash_info& cash_info) = 0;

     virtual void get_character_info(
      gbp::ns_api::ns_bots::ns_admin_reply::miscellaneous::e_result result
      , const gbp::ns_api::ns_bots::character_info_test& character_info) = 0;

     virtual void get_day_fin_plan(
      gbp::ns_api::ns_bots::ns_admin_reply::miscellaneous::e_result result
      , const gbp::ns_api::fin_plan_day_item& fin_plan) = 0;

     virtual void get_countries_list(
      gbp::ns_api::ns_bots::ns_admin_reply::miscellaneous::e_result result
      , const std::vector<gbp::ns_api::country_item>& countries) = 0;

     virtual void player_close_table(
      gbp::ns_api::ns_bots::ns_admin_reply::miscellaneous::e_result result
      , gbp_i64 tournament_id
      , gbp_i64 game_id) = 0;
 
    };

    /************************* DISTRIBUTION TEMPLATES **************************/

    struct distribution_templates
    {
     virtual ~distribution_templates() { }

     GBP_DECLARE_ENUM(
       e_templates_result
       , gbp_u8
       , friend
       , (ok)
       (unknown_error)
       (appropriate_template_not_exists)
      )

     virtual void get_templates(
      gbp::ns_api::ns_bots::ns_admin_reply::distribution_templates::e_templates_result result
      , const std::vector<gbp::ns_api::ns_bots::ns_distr_templates::d_template>& templates
      , gbp::ns_api::ns_bots::ns_distr_templates::e_game_type templates_game_type) = 0;

     virtual void add_template(
      gbp::ns_api::ns_bots::ns_admin_reply::distribution_templates::e_templates_result result
      , gbp::ns_api::ns_bots::ns_distr_templates::e_game_type templates_game_type) = 0;

     virtual void remove_template(
      gbp::ns_api::ns_bots::ns_admin_reply::distribution_templates::e_templates_result result
      , gbp::ns_api::ns_bots::ns_distr_templates::e_game_type templates_game_type) = 0;

     // ответ на назначение юзеру шаблона
     virtual void set_distribution_template(
      gbp::ns_api::ns_bots::ns_admin_reply::distribution_templates::e_templates_result result
      , gbp::ns_api::ns_bots::ns_distr_templates::e_game_item_type game_item_type
      , gbp_i64 template_id) = 0;

    };

    /***************************** AUTHORIZATION ******************************/

    struct authorization
    {
     virtual ~authorization() { }

     GBP_DECLARE_ENUM(
       e_login
       , gbp_u8
       , friend
       , (ok)
       // юзер заблокирован
       (blocked_as_user)
       // блокирован по железу
       (blocked_hardware)
       // уже залогинен
       (already_logged)
       (unknown_error)
      )

     virtual void login(
      const gbp::ns_api::ns_bots::account_info& acc_info
      , gbp::ns_api::ns_bots::ns_admin_reply::authorization::e_login result) = 0;

    };

    /******************************* ACCOUNTS *********************************/

    struct accounts
    {
     virtual ~accounts() { }

     GBP_DECLARE_ENUM(
       e_accounts
       , gbp_u8
       , friend
       , (ok)
       (unknown_error)
      )

     virtual void get_accounts(
      const std::vector<gbp::ns_api::ns_bots::account_info>& accounts
      , bool is_locked
      , gbp_i64 owner_id
      , const std::string& owner_nickname) = 0;

     virtual void add_account(gbp::ns_api::ns_bots::ns_admin_reply::accounts::e_accounts result) = 0;

     virtual void block_account(gbp::ns_api::ns_bots::ns_admin_reply::accounts::e_accounts result) = 0;

     virtual void unblock_account(gbp::ns_api::ns_bots::ns_admin_reply::accounts::e_accounts result) = 0;

     virtual void remove_account(gbp::ns_api::ns_bots::ns_admin_reply::accounts::e_accounts result) = 0;

     virtual void get_hardwares(const std::vector<gbp::ns_api::ns_bots::account_hardware>& hardwares) = 0;

     virtual void block_hardware(gbp::ns_api::ns_bots::ns_admin_reply::accounts::e_accounts result) = 0;

     virtual void activate_hardware(gbp::ns_api::ns_bots::ns_admin_reply::accounts::e_accounts result) = 0;

     virtual void change_hardware_place_name(gbp::ns_api::ns_bots::ns_admin_reply::accounts::e_accounts result) = 0;

    };

    /******************************** CASHES **********************************/

    struct cashes
    {
     virtual ~cashes() { }

     GBP_DECLARE_ENUM(
       e_cashes
       , gbp_u8
       , friend
       , (ok)
       (unknown_error)
      )

     virtual void get_cashes(
      gbp::ns_api::ns_bots::ns_admin_reply::cashes::e_cashes result
      , const std::vector<gbp::ns_api::ns_bots::cash>& cashes) = 0;

    };

    /***************************** TOURNAMENTS ********************************/

    struct tournaments
    {
     virtual ~tournaments() { }

     GBP_DECLARE_ENUM(
       e_tournaments
       , gbp_u8
       , friend
       , (ok)
       (unknown_error)
      )

     virtual void get_tournaments(
      gbp::ns_api::ns_bots::ns_admin_reply::tournaments::e_tournaments result
      , const std::vector<gbp::ns_api::ns_bots::tournament>& tournaments) = 0;

     virtual void get_additional_tournament_stat(
      gbp::ns_api::ns_bots::ns_admin_reply::tournaments::e_tournaments result
      , const gbp::ns_api::ns_bots::additional_tournament_stat& tournament_stat) = 0;

    };

    /****************************** CASH RULES ********************************/

    struct cash_rules
    {
     virtual ~cash_rules() { }

     GBP_DECLARE_ENUM(
       e_cash_rules
       , gbp_u8
       , friend
       , (ok)
       (unknown_error)
      )

     virtual void get_cash_rules(
      gbp::ns_api::ns_bots::ns_admin_reply::cash_rules::e_cash_rules result
      , const gbp::ns_api::ns_bots::cash_rules& cash_rules) = 0;

     virtual void set_cash_rules(
      gbp::ns_api::ns_bots::ns_admin_reply::cash_rules::e_cash_rules result
      , gbp::ns_api::ns_bots::cash_rules::e_game_type type_) = 0;

     virtual void get_cashes_occupancy(
      gbp::ns_api::ns_bots::ns_admin_reply::cash_rules::e_cash_rules result
      , const gbp::ns_api::ns_bots::cashes_occupancy& cashes_occupancy) = 0;

     virtual void set_cashes_occupancy(
      gbp::ns_api::ns_bots::ns_admin_reply::cash_rules::e_cash_rules result
      , gbp::ns_api::ns_bots::cash_rules::e_game_type type_) = 0;

    };

    /**************************** TOURNAMENTS RULES ****************************/

    struct tournament_rules
    {
     virtual ~tournament_rules() { }

     GBP_DECLARE_ENUM(
       e_tournament_rules
       , gbp_u8
       , friend
       , (ok)
       (unknown_error)
      )

     virtual void get_tournament_rules(
      gbp::ns_api::ns_bots::ns_admin_reply::tournament_rules::e_tournament_rules result
      , const gbp::ns_api::ns_bots::tournament_rules& tournament_rules) = 0;

     virtual void set_tournament_rules(gbp::ns_api::ns_bots::ns_admin_reply::tournament_rules::e_tournament_rules result) = 0;

    };

    /******************************** SIT_N_GO ********************************/

    struct sit_n_go_rules
    {
     GBP_DECLARE_ENUM(
       e_sit_n_go_rules
       , gbp_u8
       , friend
       , (ok)
       (unknown_error)
      )

     virtual void get_sit_n_go_rules(
      gbp::ns_api::ns_bots::ns_admin_reply::sit_n_go_rules::e_sit_n_go_rules result
      , const gbp::ns_api::ns_bots::sit_n_go_rules& rules) = 0;

     virtual void set_sit_n_go_rules(gbp::ns_api::ns_bots::ns_admin_reply::sit_n_go_rules::e_sit_n_go_rules result) = 0;

    };

    /****************************** CHARACTERS ********************************/

    struct characters
    {
     virtual ~characters() { }

     GBP_DECLARE_ENUM(
       e_characters
       , gbp_u8
       , friend
       , (ok)
       (unknown_error)
      )

     virtual void get_characters(
      gbp::ns_api::ns_bots::ns_admin_reply::characters::e_characters result
      , const std::vector<gbp::ns_api::ns_bots::character>& characters
      , bool is_locked
      , gbp_i64 owner_id
      , const std::string& owner_nickname) = 0;

     virtual void get_character_info(
      gbp::ns_api::ns_bots::ns_admin_reply::characters::e_characters result
      , const gbp::ns_api::ns_bots::character_info& info) = 0;

     virtual void get_countries_info(
      gbp::ns_api::ns_bots::ns_admin_reply::characters::e_characters result
      , const std::vector<gbp::ns_api::ns_bots::country_info_t>& countries) = 0;

     // first - страна, second - кол-во ботов по ней
     virtual void add_character(
      gbp::ns_api::ns_bots::ns_admin_reply::characters::e_characters result
      , const std::unordered_map<gbp_i32, gbp_i32>& result_info) = 0;

     virtual void remove_character(gbp::ns_api::ns_bots::ns_admin_reply::characters::e_characters result) = 0;

     virtual void update_character_info(gbp::ns_api::ns_bots::ns_admin_reply::characters::e_characters result) = 0;

    };

    /************************** PLANNED DISTRIBUTIONS *************************/

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
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp::ns_api::ns_bots::ns_admin_reply::planned_distributions::e_result result
      // gbp_i64 - номер раздачи
      , const std::unordered_map<gbp_i64, ns_api::ns_bots::planned_distribution>& planned_distributions
      // игроки за столом на текущий момент
      , const std::vector<gbp::ns_api::ns_bots::player>& players
      // номер текущей раздачи стола
      , gbp_i64 current_distribution) = 0;

     virtual void add_planned_distribution(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp::ns_api::ns_bots::ns_admin_reply::planned_distributions::e_result result) = 0;

     virtual void remove_planned_distribution(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp::ns_api::ns_bots::ns_admin_reply::planned_distributions::e_result result) = 0;

     virtual void get_planned_distr_history_items(
      gbp::ns_api::ns_bots::ns_admin_reply::planned_distributions::e_result result
      , gbp_i64 tournament_id
      , bool is_sit_n_go
      , gbp_i64 game_id
      , const std::vector<gbp::ns_api::ns_bots::planned_distr_templ_history_item>& items) = 0;

     virtual void remove_planned_distr_history_item(
      gbp::ns_api::ns_bots::ns_admin_reply::planned_distributions::e_result result
      , gbp_i64 tournament_id
      , bool is_sit_n_go
      , gbp_i64 game_id) = 0;

    };

    /********************************* LOCKS **********************************/

    struct locks
    {
     virtual ~locks() { }

     GBP_DECLARE_ENUM(
       e_locks
       , gbp_u8
       , friend
       , (ok)
       (unknown_error)
       (already_locked)
       (already_unlocked)
      )

     virtual void lock_table(
      gbp::ns_api::ns_bots::ns_locks::e_table table
      , gbp::ns_api::ns_bots::ns_admin_reply::locks::e_locks result) = 0;

     virtual void unlock_table(
      gbp::ns_api::ns_bots::ns_locks::e_table table
      , gbp::ns_api::ns_bots::ns_admin_reply::locks::e_locks result) = 0;

     virtual void lock_cash(gbp_i64 game_id, gbp::ns_api::ns_bots::ns_admin_reply::locks::e_locks result) = 0;

     virtual void unlock_cashes(const std::vector<gbp_i64>& games_ids, gbp::ns_api::ns_bots::ns_admin_reply::locks::e_locks result) = 0;

     virtual void lock_tournament_cash(
      gbp::ns_api::ns_bots::ns_admin_reply::locks::e_locks result
      , gbp_i64 tournament_id
      , gbp_i64 game_id) = 0;

     virtual void unlock_tournament_cashes(
      gbp::ns_api::ns_bots::ns_admin_reply::locks::e_locks result
      , const std::unordered_map<gbp_i64, std::vector<gbp_i64>>& tournament_games) = 0;

    };

    /******************************** HISTORY *********************************/

    struct history
    {
     virtual ~history() { }

     GBP_DECLARE_ENUM(
       e_history
       , gbp_u8
       , friend
       , (ok)
       (unknown_error)
      )

     virtual void get_history(
      gbp::ns_api::ns_bots::ns_admin_reply::history::e_history result
      , const std::vector<gbp::ns_api::ns_bots::history>& history) = 0;

    };

    /********************************* CHASE **********************************/

    struct chase
    {
     virtual ~chase() { }

     GBP_DECLARE_ENUM(
       e_chase
       , gbp_u8
       , friend
       , (ok)
       (unknown_error)
       (user_not_exists)
      )

     virtual void get_chase_cashes_stat(
      gbp::ns_api::ns_bots::ns_admin_reply::chase::e_chase result
      , const std::unordered_map<gbp_i64, gbp::ns_api::ns_bots::chase_cash>& chase_cashes) = 0;

     virtual void get_chase_tournament_cashes_stat(
      gbp::ns_api::ns_bots::ns_admin_reply::chase::e_chase result
      , const std::unordered_map<gbp_i64, gbp::ns_api::ns_bots::chase_tournament_cash>& chase_cashes) = 0;

     virtual void get_chase_sit_n_go_cashes_stat(
      gbp::ns_api::ns_bots::ns_admin_reply::chase::e_chase result
      , const std::unordered_map<gbp_i64, gbp::ns_api::ns_bots::chase_tournament_cash>& chase_cashes) = 0;     

     virtual void update_user_notes(gbp::ns_api::ns_bots::ns_admin_reply::chase::e_chase result, gbp_i64 user_id) = 0;
     virtual void update_user_task(gbp::ns_api::ns_bots::ns_admin_reply::chase::e_chase result, gbp_i64 user_id) = 0;
     virtual void get_tournament_prizes(gbp_i64 tournament_id, bool is_sit_n_go, const std::vector<ns_api::money>& prizes) = 0;

    };

    /********************************* ERRORS *********************************/

    struct statistic
    {
     virtual ~statistic() { }

     GBP_DECLARE_ENUM(
       e_statistic
       , gbp_u8
       , friend
       , (ok)
       (unknown_error)
      )

     virtual void get_ai_characters_stat(
      gbp::ns_api::ns_bots::ns_admin_reply::statistic::e_statistic result
      , const gbp::ns_api::ns_bots::ai_characters_stat& stat) = 0;

    };
    
    /********************************* ERRORS *********************************/

    struct errors
    {
     virtual ~errors() { }

     GBP_DECLARE_ENUM(
       e_errors
       , gbp_u8
       , friend
       , (ok)
       (unknown_error)
      )

     virtual void get_errors(
      gbp::ns_api::ns_bots::ns_admin_reply::errors::e_errors result
      , const std::vector<std::string>& errors) = 0;

     virtual void clear_errors(gbp::ns_api::ns_bots::ns_admin_reply::errors::e_errors result) = 0;

    };

   } // ns_admin_reply
  } // ns_bots
 } // ns_api
} // gbp
