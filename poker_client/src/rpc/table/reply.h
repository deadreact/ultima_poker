#pragma once

#include <QObject>
#include <rpcbase/emitter_table.h>
#include <memory>

class NetworkClient;

struct SingleIdObject;
//struct CashHandler;
struct TournHandler;

namespace ns_table {
    struct emitter_t;
} //namespace ns_table

#ifndef Q_MOC_RUN
    #include <api/table/reply.h>
#endif // !Q_MOC_RUN

struct reply_table_cash: gbp::ns_api::ns_table::ns_reply::cash
{
public:
    using e_result = gbp::ns_api::ns_table::ns_reply::e_result;
    typedef ns_table::emitter_t emitter_t;
    const emitter_t* emitter() const;
private:
    std::shared_ptr<gbp::net::object_registry> m_reg;
    bool m_isInitialized;

    void init();
public:
    explicit reply_table_cash(NetworkClient* const client);

    static const std::string& parentClassName();
    // cash interface
public:
    virtual void get_table_state(e_result result, const gbp::ns_api::ns_table::table_state_t& table_state) override;
    virtual void add_game_money(e_result status, gbp_i64 game_id) override;
    virtual void add_viewer(e_result status, gbp_i64 tournament_id, gbp_i64 game_id, gbp_i8 user_pos) override;
    virtual void add_waiter(e_result status, gbp_i64 game_id, gbp_u16 waiter_number) override;
    virtual void close_table(e_result result, gbp_i64 tournament_id, gbp_i64 game_id) override;
    virtual void from_viewer_to_player(e_result status, gbp_i64 game_id, const gbp::ns_api::ns_table::sit_on_table_info_t &info) override;
    virtual void sit_on_table(e_result status, gbp_i64 game_id, const gbp::ns_api::ns_table::sit_on_table_info_t &info) override;
    virtual void get_recharge_money(e_result status, gbp_i64 game_id, const gbp::ns_api::ns_table::recharge_money_t &money) override;
    virtual void remove_from_waiters(e_result status, gbp_i64 game_id, gbp_u16 waiters_count) override;
    virtual void send_chat_message(e_result result, gbp_i64 tournament_id, gbp_i64 game_id) override;
    virtual void set_autorecharge(e_result status, gbp_i64 game_id, bool set) override;
    virtual void set_guest_blind(e_result status, gbp_i64 game_id, bool enabled) override;
    virtual void set_show_hand(e_result status, gbp_i64 tournament_id, gbp_i64 game_id) override;
    virtual void set_sit_money(e_result status, gbp_i64 game_id, gbp::ns_api::money money_) override;
    virtual void set_sit_out(e_result status, gbp_i64 game_id, bool set) override;
    virtual void user_action(e_result status, gbp_i64 tournament_id, gbp_i64 game_id) override;
    virtual void user_online(e_result status, gbp_i64 tournament_id, gbp_i64 game_id) override;

};

// --------------------------------------- TOURNAMENT ------------------------------------------------------
// --------------------------------------- ---------- ------------------------------------------------------
struct reply_table_tournament: gbp::ns_api::ns_table::ns_reply::tournament
{
public:
    using e_result = gbp::ns_api::ns_table::ns_reply::e_result;
    typedef ns_table::emitter_t emitter_t;
    const emitter_t* emitter() const;
private:
    std::shared_ptr<gbp::net::object_registry> m_reg;
    bool m_isInitialized;

    void init();
public:
    explicit reply_table_tournament(NetworkClient* const client);

    static const std::string& parentClassName();
public:
    virtual void get_tourn_register_info(e_result status, gbp_i64 tournament_id, const ns_table::tourn_register_info &info) override;
    virtual void tournament_register(e_result status
                                   , gbp_i64 tournament_id
                                   , ns_table::e_tourn_button_type b_state
                                   , ns_table::e_tournament_status current_status) override;
    virtual void cancel_tourn_register(e_result result, gbp_i64 tournament_id, const gbp::ns_api::ns_tourn::cancel_tourn_register_t& info) override;

    virtual void request_rebuy(e_result status
                             , gbp_i64 tournament_id
                             , gbp_i64 game_id
                             , gbp::ns_api::money account_money
                             , const gbp::ns_api::rebuys_t &rebuy_info
                             , bool double_rebuy) override;

    virtual void make_rebuy(e_result status, gbp_i64 tournament_id, gbp_i64 game_id) override;
    virtual void make_addon(e_result status, gbp_i64 tournament_id, gbp_i64 game_id) override;
    virtual void tournament_get_state(e_result status, gbp_i64 tournament_id, const ns_table::tournament_state_t &tmp_tournament_state) override;

    virtual void open_own_tournament(e_result status
                                   , gbp_i64 tournament_id
                                   , gbp_i64 game_id
                                   , const ns_table::tournament_state_t &tournament_state
                                   , const ns_table::table_state_t &table_state
                                   , gbp_i32 players_count) override;
};
