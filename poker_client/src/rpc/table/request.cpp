#include <precompiled.h>
#include "request.h"
//#include "../network_client.h"
#include "reply.h"
#include <iostream>


namespace
{
    using e_tourn_button_type = gbp::ns_api::ns_statistic::e_tourn_button_type;
    using e_tournament_status = gbp::ns_api::ns_tournament_types::e_tournament_status;
    using e_result            = gbp::ns_api::ns_table::ns_reply::e_result;
}

request_table_cash::request_table_cash(NetworkClient * const client)
    : gbp::ns_api::ns_table::ns_request::cash()
    , m_client(*client)
{}


void request_table_cash::get_table_state(gbp_i64 game_id)
{
    request("get_table_state", game_id);
}

void request_table_cash::send_chat_message(gbp_i64 game_id, const std::string &msg)
{
    request("send_chat_message", game_id, msg);
}

void request_table_cash::close_table(gbp_i64 game_id)
{
    std::cout << "request " << game_id << std::endl;
    request("close_table", game_id);
}

void request_table_cash::user_action(gbp_i64 game_id, const std::pair<gbp::ns_api::ns_table::e_user_action, gbp_i64> &action)
{
    request("user_action", game_id, action);
}

void request_table_cash::user_online(gbp_i64 game_id)
{
    request("user_online", game_id);
}

void request_table_cash::sit_on_table(gbp_i64 game_id, gbp_i8 pos)
{
    request("sit_on_table", game_id, pos);
}

void request_table_cash::sit_on_table_canceled(gbp_i64 game_id)
{
    request("sit_on_table_canceled", game_id);
}

void request_table_cash::set_sit_money(gbp_i64 game_id, gbp::ns_api::money money_)
{
    request("set_sit_money", game_id, money_);
}

void request_table_cash::set_autorecharge(gbp_i64 game_id, bool set)
{
    request("set_autorecharge", game_id, set);
}

void request_table_cash::get_recharge_money(gbp_i64 game_id)
{
    request("get_recharge_money", game_id);
}

void request_table_cash::add_game_money(gbp_i64 game_id, gbp::ns_api::money money_)
{
    request("add_game_money", game_id, money_);
}

void request_table_cash::add_viewer(gbp_i64 tournament_id, gbp_i64 game_id)
{
    std::cout << "request add_viever" << std::endl;
    request("add_viewer", tournament_id, game_id);
}

void request_table_cash::set_guest_blind(gbp_i64 game_id, bool set)
{
    request("set_guest_blind", game_id, set);
}

void request_table_cash::set_sit_out(gbp_i64 game_id, bool set)
{
    request("set_sit_out", game_id, set);
}

void request_table_cash::set_show_hand(gbp_i64 game_id)
{
    request("set_show_hand", game_id);
}

void request_table_cash::from_viewer_to_player(gbp_i64 game_id, gbp_i8 pos)
{
    request("from_viewer_to_player", game_id, pos);
}

void request_table_cash::add_waiter(gbp_i64 game_id)
{
    request("add_waiter", game_id);
}

void request_table_cash::remove_from_waiters(gbp_i64 game_id)
{
    request("remove_from_waiters", game_id);
}



// --------------------------------------- TOURNAMENT ------------------------------------------------------
// --------------------------------------- ---------- ------------------------------------------------------
request_table_tournament::request_table_tournament(NetworkClient * const client)
    : gbp::ns_api::ns_table::ns_request::tournament()
    , m_client(*client)
{}


void request_table_tournament::get_tourn_register_info(gbp_i64 tournament_id)
{
    request("get_tourn_register_info", tournament_id);
}

void request_table_tournament::tournament_register(gbp_i64 tournament_id, bool by_ticket)
{
    request("tournament_register", tournament_id, by_ticket);
}

void request_table_tournament::cancel_tourn_register(gbp_i64 tournament_id)
{
    request("cancel_tourn_register", tournament_id);
}

void request_table_tournament::request_rebuy(gbp_i64 tournament_id, gbp_i64 game_id, bool double_rebuy)
{
    request("request_rebuy", tournament_id, game_id, double_rebuy);
}

void request_table_tournament::make_rebuy(gbp_i64 tournament_id, gbp_i64 game_id, bool double_rebuy)
{
    request("make_rebuy", tournament_id, game_id, double_rebuy);
}

void request_table_tournament::make_addon(gbp_i64 tournament_id, gbp_i64 game_id)
{
    request("make_addon", tournament_id, game_id);
}

void request_table_tournament::addon_dialog_canceled(gbp_i64 tournament_id, gbp_i64 game_id)
{
    request("addon_dialog_canceled", tournament_id, game_id);
}

void request_table_tournament::tournament_get_state(gbp_i64 tournament_id)
{
    request("tournament_get_state", tournament_id);
}

void request_table_tournament::open_own_tournament(gbp_i64 tournament_id)
{
    request("open_own_tournament", tournament_id);
}

void request_table_tournament::tournament_sit_out(gbp_i64 tournament_id, gbp_i64 game_id)
{
    request("tournament_sit_out", tournament_id, game_id);
}
