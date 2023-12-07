#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "types.h"
#include "../lobby_stat/types.h"
#include "../api_common.h"

namespace gbp
{
 namespace ns_api
 {
  namespace ns_table
  {
   namespace ns_reply
   {
    GBP_DECLARE_ENUM(
     e_result
     , gbp_u8
     , inline
     , (ok)
     (unknown_error)
     (bad_game_id)
     (bad_user_id)
     (bad_tournament_id)
     (chat_banned)
     (bad_money)
     (place_is_occupied)
     (already_exists)
     (already_registered)
     (not_enough_money)
     (game_on_deleting)
     (rat_mode_enabled)
     (wrong_position)
     (wrong_money)
     (more_than_max)
     (not_enough_account_money)
     (already_recharged)
     (exists)
     (exists_as_player)
     (exists_as_tournament_player)
     (not_authorized)
     (register_not_available)
     (too_many_players)
     (not_enough_raiting)
     (user_not_registered)
     (registered_in_another)
     (registered_in_main)
     (registered_by_qualifier)
     (only_for_invitees)
     (rebuys_limit)
     (chips_limit)
     (already_all_ined)
     (rebuy_already_exists)
     (double_rebuy_already_exists)
     (ok_without_waiting)
     (addon_not_available)
    )

    struct cash
    {
     virtual ~cash() { }

     // 'ok', 'bad_game_id'
     virtual void get_table_state(
      ns_api::ns_table::ns_reply::e_result result
      , const ns_api::ns_table::table_state_t& tbl_state) = 0;

     // ответ юзеру об успешности отправки сообщения
     virtual void send_chat_message(
      ns_api::ns_table::ns_reply::e_result result
      , gbp_i64 tournament_id
      , gbp_i64 game_id) = 0;

     /**
      * ответ юзеру об успешности закрытия стола
      * 'ok', 'bad_game_id' */
     virtual void close_table(
      ns_api::ns_table::ns_reply::e_result result
      , gbp_i64 tournament_id
      , gbp_i64 game_id) = 0;

     // ответ юзеру об успешности сделанного им хода
     virtual void user_action(
      ns_api::ns_table::ns_reply::e_result result
      , gbp_i64 tournament_id
      , gbp_i64 game_id) = 0;

     // ответ юзеру о статусе "онлайн"
     virtual void user_online(
      ns_api::ns_table::ns_reply::e_result result
      , gbp_i64 tournament_id
      , gbp_i64 game_id) = 0;

     // ответ о возможности регистрации в кеше
     virtual void sit_on_table(
      ns_api::ns_table::ns_reply::e_result result
      , gbp_i64 game_id
      , const sit_on_table_info_t& info) = 0;

     virtual void set_sit_money(
      ns_api::ns_table::ns_reply::e_result result
      , gbp_i64 game_id
      , ns_api::money money_) = 0;

     virtual void set_autorecharge(
      ns_api::ns_table::ns_reply::e_result result
      , gbp_i64 game_id
      , bool set) = 0;

     // ответ о возможности пополнения игровых денег
     virtual void get_recharge_money(
      ns_api::ns_table::ns_reply::e_result result
      , gbp_i64 game_id
      , const ns_api::ns_table::recharge_money_t& money) = 0;

     // ответ о возможности пополнения игровых денег
     virtual void add_game_money(
      ns_api::ns_table::ns_reply::e_result result
      , gbp_i64 game_id) = 0;

     virtual void add_viewer(
      ns_api::ns_table::ns_reply::e_result result
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      // если игрок при этом уже сидит за столом
      , gbp_i8 user_pos) = 0;

     virtual void set_guest_blind(
      ns_api::ns_table::ns_reply::e_result result
      , gbp_i64 game_id
      // текущее состояние "гостевой ставки"
      , bool enabled) = 0;

     virtual void add_waiter(
      ns_api::ns_table::ns_reply::e_result result
      , gbp_i64 game_id
      , gbp_u16 waiter_number = -1) = 0;

     virtual void remove_from_waiters(
      ns_api::ns_table::ns_reply::e_result result
      , gbp_i64 game_id
      , gbp_u16 waiters_count) = 0;

     virtual void set_sit_out(
      ns_api::ns_table::ns_reply::e_result result
      , gbp_i64 game_id
      , bool set) = 0;

     virtual void set_show_hand(
      ns_api::ns_table::ns_reply::e_result result
      , gbp_i64 tournament_id
      , gbp_i64 game_id) = 0;

     virtual void from_viewer_to_player(
      ns_api::ns_table::ns_reply::e_result result
      , gbp_i64 game_id
      , const ns_api::ns_table::sit_on_table_info_t& info) = 0;

    }; // cash

    struct tournament
    {
     virtual ~tournament() { }

     virtual void get_tourn_register_info(
      ns_api::ns_table::ns_reply::e_result result
      , gbp_i64 tournament_id
      , const ns_api::ns_table::tourn_register_info& info) = 0;

     virtual void tournament_register(
      ns_api::ns_table::ns_reply::e_result result
      , gbp_i64 tournament_id
      , ns_api::ns_statistic::e_tourn_button_type b_state
      , ns_api::ns_tournament_types::e_tournament_status current_status) = 0;

     virtual void cancel_tourn_register(
      ns_api::ns_table::ns_reply::e_result result
      , gbp_i64 tournament_id
      , const ns_api::ns_tourn::cancel_tourn_register_t& info) = 0;
     
     virtual void request_rebuy(
      ns_api::ns_table::ns_reply::e_result result
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , ns_api::money account_money
      , const ns_api::rebuys_t& rebuy_info
      , bool double_rebuy) = 0;

     virtual void make_rebuy(
      ns_api::ns_table::ns_reply::e_result result
      , gbp_i64 tournament_id
      , gbp_i64 game_id) = 0;

     virtual void make_addon(
      ns_api::ns_table::ns_reply::e_result result
      , gbp_i64 tournament_id
      , gbp_i64 game_id) = 0;

     virtual void tournament_get_state(
      ns_api::ns_table::ns_reply::e_result result
      , gbp_i64 tournament_id
      , const tournament_state_t& tmp_tournament_state) = 0;

     virtual void open_own_tournament(
      ns_api::ns_table::ns_reply::e_result result
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , const ns_api::ns_table::tournament_state_t& tournament_state
      , const ns_api::ns_table::table_state_t& table_state
      , gbp_i32 players_count) = 0;

    }; // struct tournament

    // flash-poker
    struct flash_poker
    {
     virtual ~flash_poker() { }

     virtual void open_flash_cash(ns_api::ns_table::ns_reply::e_result result, gbp_i64 flash_cash_id) = 0;

    }; // struct flash_poker

   } // namespace ns_reply
  } // namespace ns_table
 } // namespace ns_api
} // namespace gbp