#pragma once
 
#include <QtCore/QObject>
//#include <api/table/reply.h>
#include "../../tools/tools/types_fwd.hpp"

//! @namespace ns_table
namespace ns_table
{
    //! @class emitter_t
    class emitter_t : public QObject
    {
        Q_OBJECT
    public:
        explicit emitter_t(QObject *parent = 0);
    signals:
        void get_table_state(      e_status status, const table_state_t&) const;
        void add_game_money(       e_status status,  gbp_i64 game_id) const;
        void set_guest_blind(      e_status status,  gbp_i64 game_id, bool set) const;
        void set_autorecharge(     e_status status,  gbp_i64 game_id, bool set) const;
        void set_sit_out(          e_status status,  gbp_i64 game_id, bool set) const;
        void add_waiter(           e_status status,  gbp_i64 game_id, gbp_u16 waiter_number) const;
        void remove_from_waiters(  e_status status,  gbp_i64 game_id, gbp_u16 waiters_count) const;
        void set_sit_money(        e_status status,  gbp_i64 game_id, gbp_i64 money_) const;
        void from_viewer_to_player(e_status status,  gbp_i64 game_id, const sit_on_table_info_t &info) const;
        void sit_on_table(         e_status status,  gbp_i64 game_id, const sit_on_table_info_t &info) const;
        void get_recharge_money(   e_status status,  gbp_i64 game_id, const recharge_money_t &money) const;

        void close_table(          e_status status, const GameId& gameId) const;
        void set_show_hand(        e_status status, const GameId& gameId) const;
        void user_action(          e_status status, const GameId& gameId) const;
        void user_online(          e_status status, const GameId& gameId) const;
        void send_chat_message(    e_status status, const GameId& gameId) const;
        void add_viewer(           e_status status, const GameId& gameId, gbp_i8 user_pos) const;

        // tournament
        void make_rebuy(             e_status, const GameId& gameID) const;
        void make_addon(             e_status, const GameId& gameID) const;
        void request_rebuy(          e_status, const GameId& gameID, gbp_i64 account_money, const rebuys_t &rebuy_info, bool double_rebuy) const;
        void open_own_tournament(    e_status, const GameId& gameID, const tournament_state_t&, const table_state_t&, int players_count) const;
        void cancel_tourn_register(  e_status, gbp_i64 tournament_id, const cancel_tourn_register_t& info) const;
        void get_tourn_register_info(e_status, gbp_i64 tournament_id, const tourn_register_info &info) const;
        void tournament_get_state(   e_status, gbp_i64 tournament_id, const tournament_state_t &tmp_tournament_state) const;
        void tournament_register(    e_status, gbp_i64 tournament_id, e_tourn_button_type b_state, e_tournament_status current_status) const;
    };
} //namespace reply_table


