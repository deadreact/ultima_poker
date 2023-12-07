#include "rpcbase_table.h"

#ifndef Q_MOC_RUN
    #include <api/table/request.h>
    #include <api/table/reply.h>
    #include <api/table/types.h>
#endif // !Q_MOC_RUN

#include <QBuffer>
#include <QtGui/QPixmap>
#include <QMap>
#include <unordered_map>

#include <tools/gameid.h>

namespace
{
    void killConnection(QMetaObject::Connection* conn) {
        if (conn != nullptr) {
            QObject::disconnect(*conn);
            delete conn;
        }
    }
}

namespace ns_table
{
    QString rpc_object::classname_cash()  { return QLatin1String("gbp::ns_api::ns_request::cash"); }
    QString rpc_object::classname_tournament() { return QLatin1String("gbp::ns_api::ns_request::tournament"); }

    void rpc_object::sit_on_table_canceled(gbp_i64 game_id/*, on_sit_on_table_canceled callback*/)
    {
        // if (callback)
        // {
        //     QMetaObject::Connection* conn = new QMetaObject::Connection;
        //     auto callback2 = [callback, game_id, conn](e_status status, gbp_i64 id) {
        //         if (game_id == id) {
        //             killConnection(conn);
        //             callback(status);
        //         }
        //     }
        //     *conn = QObject::connect(emitter(), &emitter_t::sit_on_table_canceled, callback2);
        // }
        request_cash()->sit_on_table_canceled(game_id);
    }

    void rpc_object::get_table_state(gbp_i64 game_id, on_get_table_state callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, game_id, conn](e_status status, const table_state_t& table_state) {
                if (game_id == table_state._game_id) {
                    killConnection(conn);
                    callback(status, table_state);
                }
            };
            *conn = QObject::connect(emitter(), &emitter_t::get_table_state, callback2);
        }
        request_cash()->get_table_state(game_id);
    }

    void rpc_object::send_chat_message(gbp_i64 game_id, const QString &msg, on_common_reply callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, game_id, conn](e_status status, const GameId& id) {
                if (game_id == id.m_tableID) {
                    killConnection(conn);
                    callback(status);
                }
            };
            *conn = QObject::connect(emitter(), &emitter_t::send_chat_message, callback2);
        }
        request_cash()->send_chat_message(game_id, msg.toStdString());
    }

    void rpc_object::close_table(gbp_i64 game_id, on_common_reply callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, game_id, conn](e_status status, const GameId& id) {
                if (game_id == id.m_tableID) {
                    killConnection(conn);
                    callback(status);
                }
            };
            *conn = QObject::connect(emitter(), &emitter_t::close_table, callback2);
        }
        request_cash()->close_table(game_id);
    }

    void rpc_object::user_action(gbp_i64 game_id, const GameAction &action, on_common_reply callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, game_id, conn](e_status status, const GameId& id) {
                if (game_id == id.m_tableID) {
                    killConnection(conn);
                    callback(status);
                }
            };
            *conn = QObject::connect(emitter(), &emitter_t::user_action, callback2);
        }
        request_cash()->user_action(game_id, action);
    }

    void rpc_object::user_online(gbp_i64 game_id, on_common_reply callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, game_id, conn](e_status status, const GameId& id) {
                if (game_id == id.m_tableID) {
                    killConnection(conn);
                    callback(status);
                }
            };
            *conn = QObject::connect(emitter(), &emitter_t::user_online, callback2);
        }
        request_cash()->user_online(game_id);
    }

    void rpc_object::sit_on_table(gbp_i64 game_id, gbp_i8 user_pos, on_sit_on_table callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, game_id, conn](e_status status, gbp_i64 id, const sit_on_table_info_t& info) {
                if (game_id == id) {
                    killConnection(conn);
                    callback(status, info);
                }
            };
            *conn = QObject::connect(emitter(), &emitter_t::sit_on_table, callback2);
        }
        request_cash()->sit_on_table(game_id, user_pos);
    }

    void rpc_object::set_sit_money(gbp_i64 game_id, gbp_i64 amount, on_set_sit_money callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, game_id, conn](e_status status, gbp_i64 id, gbp_i64 money) {
                if (game_id == id) {
                    killConnection(conn);
                    callback(status, money);
                }
            };
            *conn = QObject::connect(emitter(), &emitter_t::set_sit_money, callback2);
        }
        request_cash()->set_sit_money(game_id, amount);
    }

    void rpc_object::set_autorecharge(gbp_i64 game_id, bool set, on_set_autorecharge callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, game_id, conn](e_status status, gbp_i64 id, bool b) {
                if (game_id == id) {
                    killConnection(conn);
                    callback(status, b);
                }
            };
            *conn = QObject::connect(emitter(), &emitter_t::set_autorecharge, callback2);
        }
        request_cash()->set_autorecharge(game_id, set);
    }

    void rpc_object::get_recharge_money(gbp_i64 game_id, on_get_recharge_money callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, game_id, conn](e_status status, gbp_i64 id, const recharge_money_t& money) {
                if (game_id == id) {
                    killConnection(conn);
                    callback(status, money);
                }
            };
            *conn = QObject::connect(emitter(), &emitter_t::get_recharge_money, callback2);
        }
        request_cash()->get_recharge_money(game_id);
    }

    void rpc_object::add_game_money(gbp_i64 game_id, gbp_i64 amount, on_add_game_money callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, game_id, amount, conn](e_status status, gbp_i64 id) {
                if (game_id == id) {
                    killConnection(conn);
                    callback(status, amount);
                }
            };
            *conn = QObject::connect(emitter(), &emitter_t::add_game_money, callback2);
        }
        request_cash()->add_game_money(game_id, amount);
    }

    void rpc_object::add_viewer(const GameId& game_id, on_common_reply callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, game_id, conn](e_status status, const GameId& id, gbp_i8 /*user_pos*/) {
                if (game_id == id) {
                    killConnection(conn);
                    callback(status);
                }
            };
            *conn = QObject::connect(emitter(), &emitter_t::add_viewer, callback2);
        }
        request_cash()->add_viewer(game_id.m_tournamentID, game_id.m_tableID);
    }

    void rpc_object::set_guest_blind(gbp_i64 game_id, bool enabled, on_set_guest_blind callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, game_id, conn](e_status status, gbp_i64 id, bool b) {
                if (game_id == id) {
                    killConnection(conn);
                    callback(status, b);
                }
            };
            *conn = QObject::connect(emitter(), &emitter_t::set_guest_blind, callback2);
        }
        request_cash()->set_guest_blind(game_id, enabled);
    }

    void rpc_object::set_sit_out(gbp_i64 game_id, bool set, on_set_sit_out callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, game_id, conn](e_status status, gbp_i64 id, bool b) {
                if (game_id == id) {
                    killConnection(conn);
                    callback(status, b);
                }
            };
            *conn = QObject::connect(emitter(), &emitter_t::set_sit_out, callback2);
        }
        request_cash()->set_sit_out(game_id, set);
    }

    void rpc_object::set_show_hand(gbp_i64 game_id, on_common_reply callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, game_id, conn](e_status status, const GameId& id) {
                if (game_id == id.m_tableID) {
                    killConnection(conn);
                    callback(status);
                }
            };
            *conn = QObject::connect(emitter(), &emitter_t::set_show_hand, callback2);
        }
        request_cash()->set_show_hand(game_id);
    }

    void rpc_object::from_viewer_to_player(gbp_i64 game_id, gbp_i8 pos, on_from_viewer_to_player callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, game_id, conn](e_status status, gbp_i64 id, const sit_on_table_info_t& info) {
                if (game_id == id) {
                    killConnection(conn);
                    callback(status, info);
                }
            };
            *conn = QObject::connect(emitter(), &emitter_t::from_viewer_to_player, callback2);
        }
        request_cash()->from_viewer_to_player(game_id, pos);
    }

    void rpc_object::add_waiter(gbp_i64 game_id, on_add_waiter callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, game_id, conn](e_status status, gbp_i64 id, quint16 waiter_number) {
                if (game_id == id) {
                    killConnection(conn);
                    callback(status, waiter_number);
                }
            };
            *conn = QObject::connect(emitter(), &emitter_t::add_waiter, callback2);
        }
        request_cash()->add_waiter(game_id);
    }

    void rpc_object::remove_from_waiters(gbp_i64 game_id, on_remove_from_waiters callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, game_id, conn](e_status status, gbp_i64 id, quint16 waiters_count) {
                if (game_id == id) {
                    killConnection(conn);
                    callback(status, waiters_count);
                }
            };
            *conn = QObject::connect(emitter(), &emitter_t::remove_from_waiters, callback2);
        }
        request_cash()->remove_from_waiters(game_id);
    }


    /*
     * Tournament API
     */

    void rpc_object::addon_dialog_canceled(const GameId& game_id) {
        request_tournament()->addon_dialog_canceled(game_id.m_tournamentID, game_id.m_tableID);
    }
    void rpc_object::tournament_sit_out(const GameId& game_id) {
        request_tournament()->tournament_sit_out(game_id.m_tournamentID, game_id.m_tableID);
    }
    void rpc_object::make_addon(const GameId& game_id, on_common_reply callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, game_id, conn](e_status status, const GameId& id) {
                if (game_id == id) {
                    killConnection(conn);
                    callback(status);
                }
            };
            *conn = QObject::connect(emitter(), &emitter_t::make_addon, callback2);
        }
        request_tournament()->make_addon(game_id.m_tournamentID, game_id.m_tableID);
    }
    void rpc_object::make_rebuy(const GameId& game_id, bool double_rebuy, on_common_reply callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, game_id, double_rebuy, conn](e_status status, const GameId& id) {
                if (game_id == id) {
                    killConnection(conn);
                    callback(status);
                }
            };
            *conn = QObject::connect(emitter(), &emitter_t::make_rebuy, callback2);
        }
        request_tournament()->make_rebuy(game_id.m_tournamentID, game_id.m_tableID, double_rebuy);
    }
    void rpc_object::request_rebuy(const GameId& game_id, bool double_rebuy, on_request_rebuy callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, game_id, conn](e_status status, const GameId& id, gbp_i64 account_money, const rebuys_t &rebuy_info, bool doubleRebuy) {
                if (game_id == id) {
                    killConnection(conn);
                    callback(status, account_money, rebuy_info, doubleRebuy);
                }
            };
            *conn = QObject::connect(emitter(), &emitter_t::request_rebuy, callback2);
        }
        request_tournament()->request_rebuy(game_id.m_tournamentID, game_id.m_tableID, double_rebuy);
    }
    void rpc_object::tournament_register(gbp_i64 tournament_id, bool by_ticket, on_tournament_register callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, tournament_id, conn](e_status status, const GameId& id, e_tourn_button_type b_state, e_tournament_status current_status) {
                if (tournament_id == id.m_tournamentID) {
                    killConnection(conn);
                    callback(status, b_state, current_status);
                }
            };
            *conn = QObject::connect(emitter(), &emitter_t::tournament_register, callback2);
        }
        request_tournament()->tournament_register(tournament_id, by_ticket);
    }
    void rpc_object::get_tourn_register_info(gbp_i64 tournament_id, on_get_tourn_register_info callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, tournament_id, conn](e_status status, const GameId& id, const tourn_register_info& info) {
                if (tournament_id == id.m_tournamentID) {
                    killConnection(conn);
                    callback(status, info);
                }
            };
            *conn = QObject::connect(emitter(), &emitter_t::get_tourn_register_info, callback2);
        }
        request_tournament()->get_tourn_register_info(tournament_id);
    }
    void rpc_object::cancel_tourn_register(gbp_i64 tournament_id, on_cancel_tourn_register callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, tournament_id, conn](e_status status, const GameId& id, const cancel_tourn_register_t& info) {
                if (tournament_id == id.m_tournamentID) {
                    killConnection(conn);
                    callback(status, info);
                }
            };
            *conn = QObject::connect(emitter(), &emitter_t::cancel_tourn_register, callback2);
        }
        request_tournament()->cancel_tourn_register(tournament_id);
    }
    void rpc_object::tournament_get_state(gbp_i64 tournament_id, on_tournament_get_state callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, tournament_id, conn](e_status status, const GameId& id, const tournament_state_t& tournament_state) {
                if (tournament_id == id.m_tournamentID) {
                    killConnection(conn);
                    callback(status, tournament_state);
                }
            };
            *conn = QObject::connect(emitter(), &emitter_t::tournament_get_state, callback2);
        }
        request_tournament()->tournament_get_state(tournament_id);
    }
    void rpc_object::open_own_tournament(gbp_i64 tournament_id, on_open_own_tournament callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, tournament_id, conn](e_status status, const GameId& id, const tournament_state_t& tournament_state, const table_state_t& table_state, int players_count) {
                if (tournament_id == id.m_tournamentID) {
                    killConnection(conn);
                    callback(status, tournament_state, table_state, players_count);
                }
            };
            *conn = QObject::connect(emitter(), &emitter_t::open_own_tournament, callback2);
        }
        request_tournament()->open_own_tournament(tournament_id);
    }


} //ns_table

