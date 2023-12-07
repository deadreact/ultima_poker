#pragma once

#include "types.h"

namespace gbp
{
 namespace ns_api
 {
  namespace ns_bots
  {
   namespace ns_gameserver_request
   { // game_server -> bot_server

    /**************************** PLAY INSTEAD BOTS *****************************/

    struct play_instead_bots
    {
     virtual ~play_instead_bots() { }

     GBP_DECLARE_ENUM(
      e_play_instead_bots
      , gbp_u8
      , friend
      , (ok)
        (unknown_error)
     )

     virtual void play_instead_bots_start(
      gbp::ns_api::ns_bots::ns_gameserver_request::play_instead_bots::e_play_instead_bots result
      , gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id) = 0;

     virtual void get_table_state_instead_bots(
      gbp::ns_api::ns_bots::ns_gameserver_request::play_instead_bots::e_play_instead_bots result
      , gbp_i64 admin_id
      , gbp_i64 tournament_id
      ,gbp_i64 game_id
      , const gbp::ns_api::ns_bots::table_state_instead_bots_t& table_state) = 0;

     virtual void set_table_state_instead_bots(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , const gbp::ns_api::ns_bots::table_state_instead_bots_t& table_state) = 0;

     virtual void set_current_user(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 user_id
      , gbp::ns_api::time_s expire_time) = 0;

     virtual void add_cards(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , const gbp::ns_api::distrib_users_cards& users_cards) = 0;

     virtual void add_table_cards(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , const std::vector<gbp_i8>& table_cards) = 0;

     virtual void clear_cards(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id) = 0;

     virtual void freeze_user(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 user_id) = 0;

     virtual void put_user_in_anticipant(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 user_id
      , bool return_to_game_btn) = 0;
     
     virtual void add_user(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , const gbp::ns_api::ns_bots::table_player_info_t& user_info) = 0;

     virtual void remove_users(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , const std::vector<gbp_i64>& removed_users) = 0;
     
     virtual void set_users_cashes(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , const gbp::ns_api::ns_table::users_cashes_t& users_cashes) = 0;

     virtual void set_pots(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , const std::vector<gbp::ns_api::money>& pots) = 0;

     virtual void set_dealer(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 user_id) = 0;
 
     virtual void set_user_money(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 user_id
      , gbp::ns_api::money money_) = 0;

     virtual void end_of_trade_round(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_u8 round_number) = 0;

     virtual void end_of_distribution(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id) = 0;

     virtual void end_of_game(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id) = 0;

     virtual void set_user_online(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 user_id) = 0;

     virtual void show_hand_buttons(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 user_id) = 0;
    
     /************ tournament ************/

     virtual void level_up(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp::ns_api::money big_blind
      , gbp::ns_api::money ante
      , gbp_i16 current_round) = 0;

     virtual void rebuys_waiting(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp::ns_api::time_s waiting_time) = 0;

     virtual void tournament_break(gbp_i64 admin_id, gbp_i64 tournament_id, ns_api::time_s break_time) = 0;

     virtual void game_deleted(gbp_i64 admin_id, gbp_i64 tournament_id, gbp_i64 game_id) = 0;

    };

    // для передачи действий в диалог чата
    struct chat_table_play
    {
     virtual ~chat_table_play() { }

     GBP_DECLARE_ENUM(
      e_chat_table_play
      , gbp_u8
      , friend
      , (ok)
        (unknown_error)
     )

     // результат блокировки стола
     virtual void chat_table_play_start(
      gbp::ns_api::ns_bots::ns_gameserver_request::chat_table_play::e_chat_table_play result
      , gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , const gbp::ns_api::ns_bots::table_state_instead_bots_t& table_state) = 0;

     virtual void set_table_state(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , const gbp::ns_api::ns_bots::table_state_instead_bots_t& table_state) = 0;

     virtual void set_current_user(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 user_id
      , gbp::ns_api::time_s expire_time) = 0;

     virtual void add_cards(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , const gbp::ns_api::distrib_users_cards& users_cards) = 0;

     virtual void add_table_cards(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , const std::vector<gbp_i8>& table_cards) = 0;

     virtual void clear_cards(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id) = 0;

     virtual void freeze_user(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 user_id) = 0;

     virtual void put_user_in_anticipant(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 user_id
      , bool return_to_game_btn) = 0;
     
     virtual void add_user(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , const gbp::ns_api::ns_bots::table_player_info_t& user_info) = 0;

     virtual void remove_users(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , const std::vector<gbp_i64>& removed_users) = 0;
     
     virtual void set_users_cashes(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , const gbp::ns_api::ns_table::users_cashes_t& users_cashes) = 0;

     virtual void set_pots(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , const std::vector<gbp::ns_api::money>& pots) = 0;

     virtual void set_dealer(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 user_id) = 0;
 
     virtual void set_user_money(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 user_id
      , gbp::ns_api::money money_) = 0;

     virtual void end_of_trade_round(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_u8 round_number) = 0;

     virtual void end_of_distribution(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id) = 0;

     virtual void end_of_game(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id) = 0;

     virtual void set_user_online(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 user_id) = 0;
    
     /************ tournament ************/

     virtual void level_up(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp::ns_api::money big_blind
      , gbp::ns_api::money ante
      , gbp_i16 current_round) = 0;

     virtual void rebuys_waiting(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp::ns_api::time_s waiting_time) = 0;

     virtual void tournament_break(gbp_i64 admin_id, gbp_i64 tournament_id, ns_api::time_s break_time) = 0;

     virtual void game_deleted(gbp_i64 admin_id, gbp_i64 tournament_id, gbp_i64 game_id) = 0;

    };

    struct tournament
    {
     virtual ~tournament() { }

     virtual void tournament_start(gbp_i64 tournament_id, bool is_sit_n_go) = 0;

     // результаты завершения квалификатора
     virtual void quialifier_finish_result(const gbp::ns_api::ns_bots::quialifier_finish_result& result) = 0;

    };

    struct cash
    {
     virtual ~cash() { }

     virtual void set_distribution_number(gbp_i64 tournament_id, gbp_i64 game_id, gbp_i64 distribution_number) = 0;
     virtual void set_current_round_type(gbp_i64 tournament_id, gbp_i64 game_id, ns_api::ns_table::e_game_round_type round_type) = 0;

     virtual void set_winners(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const gbp::ns_api::ns_table::hi_low_winners& winners
      , const gbp::ns_api::ns_table::users_and_cards& users_cards
      , const std::unordered_map<gbp_i64, ns_api::ns_bots::end_of_distribution_user_info>& users_info
      , bool by_combination) = 0;

     virtual void set_user_action(
      gbp_i64 admin_id
      , gbp_i64 chat_admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , const gbp::ns_api::ns_table::user_action_info_t& info) = 0;

     virtual void set_chat_message(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , const gbp::ns_api::ns_bots::chat_message_info& info) = 0;

    };

    struct miscellaneous
    {
     virtual ~miscellaneous() { }

     // все предварительно сгенеренные карты
     virtual void set_all_cards(gbp_i64 tournament_id, gbp_i64 game_id, const ns_api::ns_bots::all_cards& cards_) = 0;

     // для вывода логов бот-сервером в бот-админку и файл
     virtual void set_loginfo(const std::string& log_info) = 0;

     // для вывода ошибок бот-сервером
     virtual void set_logerror(const std::string& log_error) = 0;

     // отключение всех бот-админов
     virtual void all_bot_admins_disconnect() = 0;

    };

   } // ns ns_gameserver_request
  } // ns ns_bots
 } // ns ns_api
} // ns gbp
