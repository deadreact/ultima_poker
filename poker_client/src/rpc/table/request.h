#pragma once

#ifndef Q_MOC_RUN
    #include <api/table/request.h>
    #include <api/table/reply.h>
    #include <rpc/network_client.h>
#endif // !Q_MOC_RUN

namespace
{
    using e_tourn_button_type = gbp::ns_api::ns_statistic::e_tourn_button_type;
    using e_tournament_status = gbp::ns_api::ns_tournament_types::e_tournament_status;
    using e_result            = gbp::ns_api::ns_table::ns_reply::e_result;

}

namespace ns_callback
{
    using on_register = std::function<void(e_result status, e_tourn_button_type bttn_state, e_tournament_status tourn_status)>;
}

struct request_table_cash : gbp::ns_api::ns_table::ns_request::cash
{
private:
    NetworkClient& m_client;
    static const std::string parentClassName() { return "gbp::ns_api::ns_table::ns_request::cash"; }
public:
    explicit request_table_cash(NetworkClient * const client);

    template <typename ...Args>
    void request(const char* methodName, gbp_i64 gameId, Args ...args);
public:
    virtual void get_table_state(                  gbp_i64 game_id) override;

    virtual void add_waiter(                       gbp_i64 game_id) override;
    virtual void remove_from_waiters(              gbp_i64 game_id) override;

    virtual void sit_on_table_canceled(            gbp_i64 game_id) override;
    virtual void sit_on_table(                     gbp_i64 game_id, gbp_i8 pos) override;
    virtual void from_viewer_to_player(            gbp_i64 game_id, gbp_i8 pos) override;
    virtual void close_table(                      gbp_i64 game_id) override;

    virtual void set_sit_money(                    gbp_i64 game_id, gbp::ns_api::money money_) override;
    virtual void add_game_money(                   gbp_i64 game_id, gbp::ns_api::money money_) override;
    virtual void get_recharge_money(               gbp_i64 game_id) override;

    virtual void set_show_hand(                    gbp_i64 game_id) override;
    virtual void user_online(                      gbp_i64 game_id) override;

    virtual void set_guest_blind(                  gbp_i64 game_id, bool set = true) override;
    virtual void set_autorecharge(                 gbp_i64 game_id, bool set = true) override;
    virtual void set_sit_out(                      gbp_i64 game_id, bool set = true) override;
    virtual void send_chat_message(                gbp_i64 game_id, const std::string &msg) override;
    virtual void user_action(                      gbp_i64 game_id, const std::pair<gbp::ns_api::ns_table::e_user_action, gbp_i64> &action) override;

    virtual void add_viewer(gbp_i64 tournament_id, gbp_i64 game_id) override;
    inline void add_viewer(const GameId& game_id);
};

void request_table_cash::add_viewer(const GameId &game_id)
{
    add_viewer(game_id.m_tournamentID, game_id.m_tableID);
}


template <typename ...Args>
void request_table_cash::request(const char* methodName, gbp_i64 gameId, Args ...args) {
    m_client.call_server(parentClassName() + "." + methodName, gameId, args...);
}

// --------------------------------------- TOURNAMENT ------------------------------------------------------
// --------------------------------------- ---------- ------------------------------------------------------
struct request_table_tournament : gbp::ns_api::ns_table::ns_request::tournament
{
private:
    NetworkClient& m_client;
    static const std::string parentClassName() { return "gbp::ns_api::ns_table::ns_request::tournament"; }

    template <typename ...Args>
    void request(const char* methodName, gbp_i64 gameId, Args ...args);
public:
    explicit request_table_tournament(NetworkClient * const client);

    virtual void get_tourn_register_info(gbp_i64 tournament_id) override;
    virtual void tournament_register(gbp_i64 tournament_id, bool by_ticket) override;
    virtual void cancel_tourn_register(gbp_i64 tournament_id) override;
    virtual void request_rebuy(gbp_i64 tournament_id, gbp_i64 game_id, bool double_rebuy) override;
    virtual void make_rebuy(gbp_i64 tournament_id, gbp_i64 game_id, bool double_rebuy) override;
    virtual void make_addon(gbp_i64 tournament_id, gbp_i64 game_id) override;
    virtual void addon_dialog_canceled(gbp_i64 tournament_id, gbp_i64 game_id) override;
    virtual void tournament_get_state(gbp_i64 tournament_id) override;
    virtual void open_own_tournament(gbp_i64 tournament_id) override;
    virtual void tournament_sit_out(gbp_i64 tournament_id, gbp_i64 game_id) override;
// ------------------ inline --------------------------------------------
    inline void request_rebuy(const GameId& gameID, bool double_rebuy);
    inline void make_rebuy(const GameId& gameID, bool double_rebuy);
    inline void make_addon(const GameId& gameID);
    inline void addon_dialog_canceled(const GameId& gameID);
    inline void tournament_sit_out(const GameId& gameID);

// ---------------------------------------------------------------------
};

// ------------------ inline --------------------------------------------
inline void request_table_tournament::request_rebuy(const GameId& gameID, bool double_rebuy) {
    request_rebuy(gameID.m_tournamentID, gameID.m_tableID, double_rebuy);
}
inline void request_table_tournament::make_rebuy(const GameId& gameID, bool double_rebuy) {
    make_rebuy(gameID.m_tournamentID, gameID.m_tableID, double_rebuy);
}
inline void request_table_tournament::make_addon(const GameId& gameID) {
    make_addon(gameID.m_tournamentID, gameID.m_tableID);
}
inline void request_table_tournament::addon_dialog_canceled(const GameId& gameID) {
    addon_dialog_canceled(gameID.m_tournamentID, gameID.m_tableID);
}
inline void request_table_tournament::tournament_sit_out(const GameId& gameID) {
    tournament_sit_out(gameID.m_tournamentID, gameID.m_tableID);
}

template <typename ...Args>
void request_table_tournament::request(const char* methodName, gbp_i64 tournamentId, Args ...args) {
    m_client.call_server(parentClassName() + "." + methodName, tournamentId, args...);
}
