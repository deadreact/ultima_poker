#pragma once

namespace gbp {
namespace ns_api {
namespace ns_table {
namespace ns_request {
/******************* request ***********************/
struct cash;
struct tournament;
/**************************************************/
}
namespace ns_reply {
/******************** reply ***********************/
struct cash;
struct tournament;
/**************************************************/
}
}
}
}
#include "defs.h"
/******************* emitter **********************/
#include "emitter_table.h"
/**************************************************/

//! @namespace ns_table
namespace ns_table
{
    //! @class rpc_object
    struct rpc_object
    {
    public:
        //! @pure request_cash
        virtual gbp::ns_api::ns_table::ns_request::cash*  request_cash() const = 0;
        //! @pure request_tournament
        virtual gbp::ns_api::ns_table::ns_request::tournament* request_tournament() const = 0;
        //! @pure emitter
        virtual const emitter_t* emitter() const = 0;

        static QString classname_cash();
        static QString classname_tournament();
    public:


        // cash interface
    public:
        virtual void sit_on_table_canceled(gbp_i64 game_id);

        virtual void get_table_state(       gbp_i64 game_id, on_get_table_state callback = on_get_table_state());

        virtual void close_table(           gbp_i64 game_id, on_common_reply callback = on_common_reply());
        virtual void set_show_hand(         gbp_i64 game_id, on_common_reply callback = on_common_reply());
        virtual void user_action(           gbp_i64 game_id, const GameAction &action, on_common_reply callback = on_common_reply());
        virtual void user_online(           gbp_i64 game_id, on_common_reply callback = on_common_reply());
        virtual void send_chat_message(     gbp_i64 game_id, const QString &msg, on_common_reply callback = on_common_reply());
        virtual void add_viewer(            const GameId& game_id, on_common_reply callback = on_common_reply());
        
        virtual void sit_on_table(          gbp_i64 game_id, gbp_i8 user_pos, on_sit_on_table callback = on_sit_on_table());
        virtual void set_sit_money(         gbp_i64 game_id, gbp_i64 amount, on_set_sit_money callback = on_set_sit_money());
        virtual void add_game_money(        gbp_i64 game_id, gbp_i64 amount, on_add_game_money callback = on_add_game_money());
        virtual void set_autorecharge(      gbp_i64 game_id, bool set, on_set_autorecharge callback = on_set_autorecharge());
        virtual void set_sit_out(           gbp_i64 game_id, bool set, on_set_sit_out callback = on_set_sit_out());
        virtual void set_guest_blind(       gbp_i64 game_id, bool enabled, on_set_guest_blind callback = on_set_guest_blind());
        virtual void from_viewer_to_player( gbp_i64 game_id, gbp_i8 pos, on_from_viewer_to_player callback = on_from_viewer_to_player());
        virtual void get_recharge_money(    gbp_i64 game_id, on_get_recharge_money callback = on_get_recharge_money());
        virtual void add_waiter(            gbp_i64 game_id, on_add_waiter callback = on_add_waiter());
        virtual void remove_from_waiters(   gbp_i64 game_id, on_remove_from_waiters callback = on_remove_from_waiters());
        
        //tournament
        virtual void addon_dialog_canceled(   const GameId& game_id);
        virtual void tournament_sit_out(      const GameId& game_id);

        virtual void make_addon(              const GameId& game_id, on_common_reply callback = on_common_reply());
        virtual void make_rebuy(              const GameId& game_id, bool double_rebuy, on_common_reply callback = on_common_reply());
        virtual void request_rebuy(           const GameId& game_id, bool double_rebuy, on_request_rebuy callback = on_request_rebuy());
         
        virtual void tournament_register(     gbp_i64 tournament_id, bool by_ticket, on_tournament_register callback = on_tournament_register());
        virtual void get_tourn_register_info( gbp_i64 tournament_id, on_get_tourn_register_info callback = on_get_tourn_register_info());
        virtual void cancel_tourn_register(   gbp_i64 tournament_id, on_cancel_tourn_register callback = on_cancel_tourn_register());
        virtual void tournament_get_state(    gbp_i64 tournament_id, on_tournament_get_state callback = on_tournament_get_state());
        virtual void open_own_tournament(     gbp_i64 tournament_id, on_open_own_tournament callback = on_open_own_tournament());
    };
} //namespace ns_table
