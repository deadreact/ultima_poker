#pragma once

#include "types.h"

namespace gbp
{
 namespace ns_api
 {
  namespace ns_bots
  {
   namespace ns_botserver_admin_request
   { // bot_server -> bot_adminka

    struct play_instead_bots
    {
     virtual ~play_instead_bots() { }

     GBP_DECLARE_ENUM(
       e_result
       , gbp_u8
       , friend
       , (ok)
         (unknown_error)
      )

     // replies
     virtual void play_instead_bots_start(     
      gbp::ns_api::ns_bots::ns_botserver_admin_request::play_instead_bots::e_result result
      , gbp_i64 tournament_id
      , gbp_i64 game_id) = 0;

     virtual void set_status(
      gbp::ns_api::ns_bots::ns_botserver_admin_request::play_instead_bots::e_result result
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp::ns_api::ns_bots::e_play_instead_bots_status status) = 0;

     virtual void make_bot_online(
      gbp::ns_api::ns_bots::ns_botserver_admin_request::play_instead_bots::e_result result
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 bot_id) = 0;

     // requests
     virtual void set_table_state_instead_bots(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const gbp::ns_api::ns_bots::table_state_instead_bots_t& table_state) = 0;

     virtual void set_current_user(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 user_id
      , gbp::ns_api::time_s expire_time) = 0;

     virtual void set_user_action(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const gbp::ns_api::ns_table::user_action_info_t& info) = 0;

     virtual void add_cards(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const gbp::ns_api::distrib_users_cards& users_cards) = 0;

     virtual void add_table_cards(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const std::vector<gbp_i8>& table_cards) = 0;

     virtual void clear_cards(
      gbp_i64 tournament_id
      , gbp_i64 game_id) = 0;

     virtual void freeze_user(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 user_id) = 0;

     virtual void put_user_in_anticipant(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 user_id
      , bool return_to_game_btn) = 0;

     virtual void add_user(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const gbp::ns_api::ns_bots::table_player_info_t& user_info) = 0;

     virtual void remove_users(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const std::vector<gbp_i64>& removed_users) = 0;

     virtual void set_users_cashes(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const gbp::ns_api::ns_table::users_cashes_t& users_cashes) = 0;

     virtual void set_pots(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const std::vector<gbp::ns_api::money>& pots) = 0;

     virtual void set_winners(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const gbp::ns_api::ns_table::hi_low_winners& winners
      , const gbp::ns_api::ns_table::users_and_cards& users_cards
      , bool by_combination) = 0;
 
     virtual void set_dealer(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 user_id) = 0;

     virtual void set_user_money(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 user_id
      , gbp::ns_api::money money_) = 0;

     virtual void end_of_trade_round(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_u8 round_number) = 0;

     virtual void end_of_distribution(
      gbp_i64 tournament_id
      , gbp_i64 game_id) = 0;

     virtual void end_of_game(
      gbp_i64 tournament_id
      , gbp_i64 game_id) = 0;

     virtual void set_user_online(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 user_id) = 0;
      
     virtual void set_user_actions(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 bot_id
      , const std::vector<gbp::ns_api::ns_table::available_user_course_t>& actions
      , const gbp::ns_api::ns_table::money_info_t& money_info) = 0;
      
     virtual void ask_blind(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 bot_id
      , bool is_big_blind
      , gbp::ns_api::money blind) = 0;
      
     virtual void show_hand_buttons(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 bot_id) = 0;
      
     virtual void set_chat_message(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 bot_id
      , const std::string& message) = 0;
     
     virtual void show_recharge_message(
      gbp_i64 game_id
      , const gbp::ns_api::ns_table::recharge_money_t& money) = 0;

     virtual void set_distribution_number(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 distribution_number) = 0;

     /********** tournaments ***********/

     virtual void addon_available(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 bot_id
      , const ns_api::addon_t& info
      , ns_api::money account_money
      , ns_api::time_s time_left) = 0;

     // ответ об успешности выполнения аддона
     GBP_DECLARE_ENUM(
       e_make_addon
       , gbp_u8
       , friend
       , (ok)
       (unknown_error)
      )
     virtual void make_addon(
      gbp::ns_api::ns_bots::ns_botserver_admin_request::play_instead_bots::e_make_addon status
      , gbp_i64 bot_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id) = 0; 

     // ответ об успешности выполнения ребая
     GBP_DECLARE_ENUM(
       e_make_rebuy
       , gbp_u8
       , friend
       , (ok)
       (unknown_error)
      )
     virtual void make_rebuy(
      gbp::ns_api::ns_bots::ns_botserver_admin_request::play_instead_bots::e_make_rebuy status
      , gbp_i64 bot_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id) = 0;

     virtual void show_rebuy_buttons(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 bot_id
      , bool double_available) = 0;

     virtual void hide_addon(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 bot_id) = 0;

     virtual void hide_rebuy_btn(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 bot_id) = 0;

     virtual void level_up(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp::ns_api::money big_blind
      , gbp::ns_api::money ante
      , gbp_i16 current_round) = 0;

     virtual void rebuys_waiting(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp::ns_api::time_s waiting_time) = 0;

     virtual void tournament_break(
      gbp_i64 tournament_id
      , gbp::ns_api::time_s break_time) = 0;

     virtual void game_deleted(gbp_i64 tournament_id, gbp_i64 game_id) = 0;

     // предварительное отображение победителей
     virtual void set_predefined_winners(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const std::unordered_set<gbp_i64>& winners) = 0;

     virtual void bounty(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const std::vector<std::string>& nicknames
      , gbp::ns_api::money money_) = 0;

     virtual void table_change(
      gbp_i64 tournament_id
      , gbp_i64 old_game_id
      , gbp_i64 new_game_id
      , gbp_i8 new_pos
      , const gbp::ns_api::ns_table::table_state_t& table_state) = 0;

     virtual void tournament_canceled(gbp_i64 tournament_id, gbp::ns_api::money buy_in) = 0;

    };

    // chat table play
    struct chat_table_play
    {
     virtual void chat_table_play_start(
      const gbp::ns_api::ns_bots::table_state_instead_bots_t& table_state
      , gbp_i64 tournament_id
      , gbp_i64 game_id) = 0;

     virtual void set_table_state(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const gbp::ns_api::ns_bots::table_state_instead_bots_t& table_state) = 0;

     virtual void set_current_user(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 user_id
      , gbp::ns_api::time_s expire_time) = 0;

     virtual void set_user_action(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const gbp::ns_api::ns_table::user_action_info_t& info) = 0;

     virtual void add_cards(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const gbp::ns_api::distrib_users_cards& users_cards) = 0;

     virtual void add_table_cards(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const std::vector<gbp_i8>& table_cards) = 0;

     virtual void clear_cards(
      gbp_i64 tournament_id
      , gbp_i64 game_id) = 0;

     virtual void freeze_user(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 user_id) = 0;

     virtual void put_user_in_anticipant(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 user_id
      , bool return_to_game_btn) = 0;

     virtual void add_user(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const gbp::ns_api::ns_bots::table_player_info_t& user_info) = 0;

     virtual void remove_users(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const std::vector<gbp_i64>& removed_users) = 0;

     virtual void set_users_cashes(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const gbp::ns_api::ns_table::users_cashes_t& users_cashes) = 0;

     virtual void set_pots(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const std::vector<gbp::ns_api::money>& pots) = 0;

     virtual void set_winners(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const gbp::ns_api::ns_table::hi_low_winners& winners
      , const gbp::ns_api::ns_table::users_and_cards& users_cards
      , bool by_combination) = 0;

     virtual void set_dealer(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 user_id) = 0;
 
     virtual void set_user_money(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 user_id
      , gbp::ns_api::money money_) = 0;     

     virtual void end_of_trade_round(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_u8 round_number) = 0;

     virtual void end_of_distribution(
      gbp_i64 tournament_id
      , gbp_i64 game_id) = 0;

     virtual void end_of_game(
      gbp_i64 tournament_id
      , gbp_i64 game_id) = 0;

     virtual void set_user_online(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 user_id) = 0;

     /************* tournaments **************/

     virtual void tournament_break(
      gbp_i64 tournament_id
      , gbp::ns_api::time_s break_time) = 0;

     virtual void level_up(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp::ns_api::money big_blind
      , gbp::ns_api::money ante
      , gbp_i16 current_round) = 0;

     virtual void rebuys_waiting(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp::ns_api::time_s waiting_time) = 0;

     virtual void game_deleted(gbp_i64 tournament_id, gbp_i64 game_id) = 0;

     // предварительное отображение победителей
     virtual void set_predefined_winners(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const std::unordered_set<gbp_i64>& winners) = 0;

    };

    // miscellaneous
    struct miscellaneous
    {
     /**
      * высылается, когда в списках игр 
      * появляются новые люди или наблюдающие */
     virtual void set_new_users_stat(const std::vector<gbp::ns_api::ns_bots::users_stat_admin>& users_stat) = 0;
    };

    /******************************** USER LOCKS ***********************************/
    
    /**
     * вызывается бот-сервером в случае, когда кто-то заблокировал таблицу/кеш/турнир и т.п. - для всех тех, 
     * кто не совершил действия блокировки/разблокировки. 
     * То есть, для того, кто заблокировал/разблокировал шлётся ответ из типа locks, а для всех остальных 
     * в этот же момент рассылается соответствующая апиха из user_locks, чтобы они сразу 
     * увидели изменения в своих таблицах */
    struct user_locks
    {
     /**
      * кто-то заблокировал таблицу целиком.
      * owner_nickname - nickname того, кто заблокировал */
     virtual void user_locked_table(ns_api::ns_bots::ns_locks::e_table table, const std::string& owner_nickname) = 0;
     // кто-то разблокировал таблицу целиком
     virtual void user_unlocked_table(ns_api::ns_bots::ns_locks::e_table table) = 0;

     // кто-то заблокировал кеш
     virtual void user_locked_cash(gbp_i64 game_id, const std::string& owner_nickname) = 0;
     // кто-то разблокировал кеши
     virtual void user_unlocked_cashes(const std::vector<gbp_i64>& games_ids) = 0;
     
     // кто-то заблокировал турнирный кеш
     virtual void user_locked_tournament_cash(gbp_i64 tournament_id, gbp_i64 game_id, const std::string& owner_nickname) = 0;

     /**
      * кто-то разблокировал турнирные кеши.
      * first - ID турнира
      * second - ID кеша */
     virtual void user_unlocked_tournament_cashes(const std::unordered_map<gbp_i64, std::vector<gbp_i64>>& tournament_games) = 0;

    };

    /*********************************** CHATS *************************************/

    struct chats
    {
     /**
      * отправка админу нового сообщения.
      * Если этот стол заблокирован, то в списке столов никак не выделять.
      * Если этот стол пришёл админу, заблокировавшему его, 
      * то выделить этот стол в диалоге чат-кешей у этого админа  */
     virtual void add_chat_message(const std::pair<gbp::ns_api::ns_bots::chat_message_info, gbp::ns_api::ns_bots::chat_cash>& chat_message) = 0;

     // какой-то админ заблокировал чат-кеш
     virtual void lock_chat_cash(gbp_i64 chat_cash_id, gbp_i64 admin_id, const std::string& admin_nickname) = 0;

     // какой-то админ разблокировал чат-кеши
     virtual void unlock_chat_cashes(const std::vector<gbp_i64>& chat_cashes_ids) = 0;

     // админу шлётся список чат-кешей
     virtual void get_chat_cashes(const std::unordered_map<gbp_i64, gbp::ns_api::ns_bots::chat_cash>& cashes) = 0;

     // админу шлётся список сообщений по ID стола
     virtual void get_chat_messages(gbp_i64 chat_cash_id, const std::vector<gbp::ns_api::ns_bots::chat_message_info>& messages) = 0;

    };

    /*********************************** TESTS *************************************/

    struct tests {
     virtual void send_test_message(const std::string& message) = 0;
    };

   } // ns_botserver_admin_request
  } // ns_bots
 } // ns_api
} // gbp