#pragma once

#include "../api_common.h"

namespace gbp
{
 namespace ns_api
 {
  namespace ns_history
  {

   /***************************************************************************/

   // енум действий юзеров
   GBP_DECLARE_ENUM(
    e_user_action
    , gbp_u8
    , inline
    ,
    (fold)
    (check)
    (bet)
    (call)
    (raise)
    (all_in)
    (small_blind)
    (big_blind)
    (bring_in)
    (guest_bet)
    (ante)
   )

   // отображение карт по раздаче
   GBP_DECLARE_ENUM(
    e_show_cards
    , gbp_u8
    , inline
    ,
    // выигрыш не по комбинации, не показывать карты победителя
    (type1)
    /**
     * выигрыш по комбинации и это кеш. Показывать карты только 
     * тех игроков, которые выиграли деньги (разница между стеками начала 
     * раздачи и концом положительная) */
    (type2)
    // это турнир и выигрышь по комбинации, показывать карты всех
    (type3)
   )

   /***************************************************************************/

   GBP_DECLARE_TYPE(
    action_info
    ,
    (action, (e_user_action))
    (money, (gbp::ns_api::money))
    (time, (gbp::ns_api::time_s))
    (current_round, (gbp_i8))
    )

   GBP_DECLARE_TYPE(
    user_info
    ,
    (user_id, (gbp_i64))
    (nickname, (std::string))
    (email, (std::string))
    (locale, (std::string))
    // позиция юзера за столом
    (user_pos, (gbp_i8))
    (is_bot, (bool))
    (is_admin, (bool))
    (is_dealer, (bool))
    (start_stack, (gbp::ns_api::money)) // начальный стек
    (in_game_money, (gbp::ns_api::money)) // деньги в игре
    (account_money, (gbp::ns_api::money)) // деньги аккаунта
    (tournament_place, (gbp_i32)) // место в турнире
    /**
     * разыграно денег в раздаче.
     * Это разница в деньгах между началом и концом раздачи.
     * Т.е. если у игрока на начало раздачи было 100, а в конце 110,
     * то значение будет 10. Если на начало было 100, а в конце 80 - значение будет -20
     */
    (raffled_money, (gbp::ns_api::money))
    // карты пользователя(дополняются)
    (cards, (gbp::ns_api::ns_table::cards))
    // выигрышная комбинация
    (win_cards, (gbp::ns_api::ns_table::cards))
    // енум выигрышной комбинации
    (combination, (gbp::ns_api::ns_table::e_cards_combination))
    // actions
    (actions, (std::vector<action_info>))
   )

   GBP_DECLARE_TYPE(
    distribution_info
    ,
    // id стола
    (game_id, (gbp_i64))
    // id турнира (-1, если обычный кеш)
    (tournament_id, (gbp_i64))
    // отображение карт
    (show_cards, (e_show_cards))
    // победители с деньгами
    (winners, (std::unordered_map<gbp_i64, ns_api::money>))
    // номер раздачи
    (distribution_number, (gbp_i64))
    // текущий уровень турнира, -1, если кеш
    (tournament_level, (gbp_i64))
    // -1 если обычный кеш
    (buy_in, (gbp::ns_api::money))
    // комиссия, -1 если обычный кеш
    (fee, (gbp_i8))
    // рейк -1 если турнир
    (rake, (gbp_i8))
    // большой блайнд
    (big_blind, (gbp::ns_api::money))
    // utc
    (distribution_start_time, (gbp::ns_api::time_s))
    // utc
    (distribution_finish_time, (gbp::ns_api::time_s))
    // тип игры
    (game_type, (gbp::ns_api::ns_game::e_game_type))
    // лимит игры
    (game_limit, (gbp::ns_api::ns_game::e_game_limit))
    // банки стола
    (pots, (std::vector<gbp::ns_api::money>))
    // карты стола
    (table_cards, (gbp::ns_api::ns_table::cards))
    // кол-во мест стола
    (seats_count, (gbp_u8))
    // фрирольный кеш или нет
    (is_freeroll, (bool))
    // турнирный анте
    (ante, (gbp::ns_api::money))
    /**
     * места (0-10(макс.)), юзеры на них и стартовые
     * стеки юзеров на начало раздачи
     */
    // user_id : user_info
    (players, (std::unordered_map<gbp_i64, user_info>))
    )

   /***************************************************************************/

   GBP_DECLARE_ENUM(
    e_history_status
    , gbp_u8
    , inline
    ,
    (ok)
    (unknown_error)
   )

   namespace ns_request
   { // user -> server

    GBP_DECLARE_TYPE(
     get_history_filter
     ,
     (user_id, (gbp_i64))
     (user_nickname, (std::string))
     (user_locale, (std::string))
     (user_email, (std::string))
     (gt, (gbp::ns_api::ns_game::e_game_type))
     (tt, (gbp::ns_api::ns_tournament_types::e_tournament_type))
     (cache_or_tournament, (bool)) // true for caches
     (table_id, (gbp_i64))
     (from_time, (gbp::ns_api::time_s))
     (to_time, (gbp::ns_api::time_s))
     (limit, (gbp_u32))
     )

    struct history
    {
     virtual ~history() { }

     virtual void put_distribution_info(const distribution_info &di) = 0;
     virtual void put_login_logout_ip_datetime(
      gbp_i64 user_id
      , gbp::ns_api::time_s login_time
      , gbp::ns_api::time_s logout_time
      , const std::string &ip
      ) = 0;
     virtual void get_history(const get_history_filter& filter) = 0;
    };

   } // ns ns_request

   /***************************************************************************/

  } // ns ns_history
 } // ns ns_api
} // ns gbp