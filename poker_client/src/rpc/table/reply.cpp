#include <precompiled.h>
#include <QMetaMethod>
#include "reply.h"
#include "../network_client.h"
#include <iostream>

#define endline std::endl
namespace tmp {
    static auto& output  = std::cout;
} // tmp

namespace ns_table
{
    emitter_t::emitter_t(QObject *parent)
        : QObject(parent)
    {}

} //namespace reply_table

/*static*/ const std::string &reply_table_cash::parentClassName()
{
    static const std::string className = "gbp::ns_api::ns_table::ns_reply::cash";
    return className;
}


reply_table_cash::reply_table_cash(NetworkClient * const client)
    : m_reg(client->create_registry(parentClassName()))
    , m_isInitialized(false)
{
    init();

}

const reply_table_cash::emitter_t *reply_table_cash::emitter() const
{
    static const emitter_t emitter;
    return &emitter;
}

void reply_table_cash::init()
{
    if (!m_isInitialized)
        {
            m_reg->on("get_table_state",       this, &reply_table_cash::get_table_state);
            m_reg->on("send_chat_message",     this, &reply_table_cash::send_chat_message);
            m_reg->on("close_table",           this, &reply_table_cash::close_table);
            m_reg->on("user_action",           this, &reply_table_cash::user_action);
            m_reg->on("user_online",           this, &reply_table_cash::user_online);
            m_reg->on("sit_on_table",          this, &reply_table_cash::sit_on_table);
            m_reg->on("set_sit_money",         this, &reply_table_cash::set_sit_money);
            m_reg->on("set_autorecharge",      this, &reply_table_cash::set_autorecharge);
            m_reg->on("get_recharge_money",    this, &reply_table_cash::get_recharge_money);
            m_reg->on("add_game_money",        this, &reply_table_cash::add_game_money);
            m_reg->on("add_viewer",            this, &reply_table_cash::add_viewer);
            m_reg->on("set_guest_blind",       this, &reply_table_cash::set_guest_blind);
            m_reg->on("add_waiter",            this, &reply_table_cash::add_waiter);
            m_reg->on("remove_from_waiters",   this, &reply_table_cash::remove_from_waiters);
            m_reg->on("set_sit_out",           this, &reply_table_cash::set_sit_out);
            m_reg->on("set_show_hand",         this, &reply_table_cash::set_show_hand);
            m_reg->on("from_viewer_to_player", this, &reply_table_cash::from_viewer_to_player);

            m_isInitialized = true;
        }
}

void reply_table_cash::get_table_state(e_result result, const gbp::ns_api::ns_table::table_state_t& table_state) {
    GameId gameId(table_state._game_id, table_state._tournament_id);

    emit emitter()->get_table_state(result, table_state);
}

void reply_table_cash::send_chat_message(e_result result, gbp_i64 tournament_id, gbp_i64 game_id) {
    GameId gameId(game_id, tournament_id);

    emit emitter()->send_chat_message(result, gameId);
}

void reply_table_cash::close_table(e_result result, gbp_i64 tournament_id, gbp_i64 game_id) {
    GameId gameId(game_id, tournament_id);
    std::cout << "reply" << result << " " << gameId.toString().toStdString() << std::endl;
    emit emitter()->close_table(result, gameId);
}

void reply_table_cash::user_action(e_result status, gbp_i64 tournament_id, gbp_i64 game_id) {
    GameId gameId(game_id, tournament_id);
    emit emitter()->user_action(status, gameId);
}

void reply_table_cash::user_online(e_result status, gbp_i64 tournament_id, gbp_i64 game_id) {
    GameId gameId(game_id, tournament_id);
    emit emitter()->user_online(status, gameId);
}

void reply_table_cash::sit_on_table(e_result status, gbp_i64 game_id, const gbp::ns_api::ns_table::sit_on_table_info_t &info) {
    emit emitter()->sit_on_table(status, game_id, info);
}

void reply_table_cash::set_sit_money(e_result status, gbp_i64 game_id, gbp::ns_api::money money_) {
    emit emitter()->set_sit_money(status, game_id, money_);
}

void reply_table_cash::set_autorecharge(e_result status, gbp_i64 game_id, bool set) {
    emit emitter()->set_autorecharge(status, game_id, set);
}

void reply_table_cash::get_recharge_money(e_result status, gbp_i64 game_id, const gbp::ns_api::ns_table::recharge_money_t &money) {
    emit emitter()->get_recharge_money(status, game_id, money);
}

void reply_table_cash::add_game_money(e_result status, gbp_i64 game_id) {
    emit emitter()->add_game_money(status, game_id);
}

void reply_table_cash::add_viewer(e_result status, gbp_i64 tournament_id, gbp_i64 game_id, gbp_i8 user_pos) {
    GameId gameId(game_id, tournament_id);
    emit emitter()->add_viewer(status, gameId, user_pos);
}

void reply_table_cash::set_guest_blind(e_result status, gbp_i64 game_id, bool enabled) {
    emit emitter()->set_guest_blind(status, game_id, enabled);
}

void reply_table_cash::add_waiter(e_result status, gbp_i64 game_id, gbp_u16 waiter_number) {
    emit emitter()->add_waiter(status, game_id, waiter_number);
}

void reply_table_cash::remove_from_waiters(e_result status, gbp_i64 game_id, gbp_u16 waiters_count) {
    emit emitter()->remove_from_waiters(status, game_id, waiters_count);
}

void reply_table_cash::set_sit_out(e_result status, gbp_i64 game_id, bool set) {
    emit emitter()->set_sit_out(status, game_id, set);

}

void reply_table_cash::set_show_hand(e_result status, gbp_i64 tournament_id, gbp_i64 game_id) {
    GameId gameId(game_id, tournament_id);
    emit emitter()->set_show_hand(status, gameId);
}

void reply_table_cash::from_viewer_to_player(e_result status, gbp_i64 game_id, const gbp::ns_api::ns_table::sit_on_table_info_t &info) {
    tmp::output << "reply from_viewer_to_player\nstatus:" << status << ", game id: " << game_id << ", sit_on_table_info: " << info << endline;
    emit emitter()->from_viewer_to_player(status, game_id, info);
}

// --------------------------------------- TOURNAMENT ------------------------------------------------------
// --------------------------------------- ---------- ------------------------------------------------------
/*static*/ const std::string &reply_table_tournament::parentClassName()
{
    static const std::string className = "gbp::ns_api::ns_table::ns_reply::tournament";
    return className;
}


reply_table_tournament::reply_table_tournament(NetworkClient * const client)
    : m_reg(client->create_registry(parentClassName()))
//    , m_emitter(client)
    , m_isInitialized(false)
{
    init();
}

const reply_table_tournament::emitter_t *reply_table_tournament::emitter() const
{
    static const emitter_t emitter;
    return &emitter;
}

void reply_table_tournament::init()
{
    if (!m_isInitialized)
        {
            m_reg->on("get_tourn_register_info", this, &reply_table_tournament::get_tourn_register_info);
            m_reg->on("tournament_register",     this, &reply_table_tournament::tournament_register);
            m_reg->on("cancel_tourn_register",   this, &reply_table_tournament::cancel_tourn_register);
            m_reg->on("request_rebuy",           this, &reply_table_tournament::request_rebuy);
            m_reg->on("make_rebuy",              this, &reply_table_tournament::make_rebuy);
            m_reg->on("make_addon",              this, &reply_table_tournament::make_addon);
            m_reg->on("tournament_get_state",    this, &reply_table_tournament::tournament_get_state);
            m_reg->on("open_own_tournament",     this, &reply_table_tournament::open_own_tournament);

            m_isInitialized = true;
        }
}

void reply_table_tournament::get_tourn_register_info(e_result status, gbp_i64 tournament_id, const ns_table::tourn_register_info &info)
{
    emit emitter()->get_tourn_register_info(status, tournament_id, info);
}

void reply_table_tournament::tournament_register(e_result status
                                               , gbp_i64 tournament_id
                                               , ns_table::e_tourn_button_type b_state
                                               , ns_table::e_tournament_status current_status)
{
    tmp::output << "reply_table_tournament::tournament_register(" << status << ")" << endline; 
    emit emitter()->tournament_register(status, tournament_id, b_state, current_status);
}

void reply_table_tournament::cancel_tourn_register(e_result result, gbp_i64 tournament_id, const gbp::ns_api::ns_tourn::cancel_tourn_register_t &info)
{
    tmp::output << "reply_table_tournament::cancel_tourn_register(" << result << ")" << endline; 
    emit emitter()->cancel_tourn_register(result, tournament_id, info);
}

void reply_table_tournament::request_rebuy(e_result status
                                         , gbp_i64 tournament_id
                                         , gbp_i64 game_id
                                         , gbp::ns_api::money account_money
                                         , const gbp::ns_api::rebuys_t &rebuy_info
                                         , bool double_rebuy)
{
    tmp::output << "reply_table_tournament::request_rebuy(" << status << ")" << endline; 
    GameId gameID(game_id, tournament_id);
    emit emitter()->request_rebuy(status, gameID, account_money, rebuy_info, double_rebuy);
}

void reply_table_tournament::make_rebuy(e_result status, gbp_i64 tournament_id, gbp_i64 game_id)
{
    tmp::output << "reply_table_tournament::make_rebuy(" << status << ")" << endline; 
    GameId gameID(game_id, tournament_id);
    emit emitter()->make_rebuy(status, gameID);
}

void reply_table_tournament::make_addon(e_result status, gbp_i64 tournament_id, gbp_i64 game_id)
{
    tmp::output << "reply_table_tournament::make_addon(" << status << ")" << endline; 
    GameId gameID(game_id, tournament_id);
    emit emitter()->make_addon(status, gameID);
}

void reply_table_tournament::tournament_get_state(e_result status
                                                , gbp_i64 tournament_id
                                                , const ns_table::tournament_state_t &tmp_tournament_state)
{
    tmp::output << "reply_table_tournament::tournament_get_state(" << status << ")" << endline; 
    emit emitter()->tournament_get_state(status, tournament_id, tmp_tournament_state);
}

void reply_table_tournament::open_own_tournament(e_result status
                                               , gbp_i64 tournament_id
                                               , gbp_i64 game_id
                                               , const ns_table::tournament_state_t &tournament_state
                                               , const ns_table::table_state_t &table_state
                                               , gbp_i32 players_count)
{
    tmp::output << "reply_table_tournament::open_own_tournament(" << status << ")" << endline; 
    GameId gameID(game_id, tournament_id);
    emit emitter()->open_own_tournament(status, tournament_id, tournament_state, table_state, players_count);
}
