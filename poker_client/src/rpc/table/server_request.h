#pragma once

#include <QObject>
#include <memory>
#include <tools/types_fwd.hpp>

#ifndef Q_MOC_RUN
    #include <api/table/server_request.h>
#endif // Q_MOC_RUN

class NetworkClient;
class DLog;
struct GameId;

namespace
{
    using time_s = gbp::ns_api::time_s;
}

namespace ns_server_request
{
    class emitter_table : public QObject
    {
        Q_OBJECT
    public:
        explicit emitter_table(QObject *parent = 0) : QObject(parent) {}

    signals:
//---------------- cash signals
        void open_cashes(				const std::vector<ns_table::table_state_t> &cashes) const;

        void show_guest_buttons(		gbp_i64 game_id, bool) const;
        void set_new_waiting_position(	gbp_i64 game_id, uint waiters_count, quint16 w_pos) const;
        void set_table_state(			gbp_i64 game_id, const ns_table::table_state_t &table_state) const;
        void show_recharge_message(		gbp_i64 game_id, const ns_table::recharge_money_t &money) const;
        //! @brief requested when current player sits on table from waiter
        void open_table(				gbp_i64 game_id, const ns_table::sit_on_table_info_t &info
                                                       , const ns_table::table_state_t &table_state) const;
        void clear_cards(				const GameId& game_id) const;
        void clear_common_pot(			const GameId& game_id) const;
        void set_common_pot(			const GameId& game_id, gbp_i64 pot) const;
        void close_table(				const GameId& game_id) const;

        void set_distribution_number(	const GameId& game_id, gbp_i64 distribution_number) const;
        void end_of_distribution(		const GameId& game_id) const;

        void set_current_round_type(	const GameId& game_id, gbp::ns_api::ns_table::e_game_round_type round_type) const;
        void end_of_trade_round(		const GameId& game_id, gbp_u8 round_number) const;

        void end_of_game(				const GameId& game_id) const;

        void show_hand_buttons(			const GameId& game_id) const;
        void set_dealer(				const GameId& game_id, gbp_i64 user_id) const;
        void set_user_online(			const GameId& game_id, gbp_i64 user_id) const;
        void freeze_user(				const GameId& game_id, gbp_i64 user_id) const;
        void put_user_in_anticipant(	const GameId& game_id, gbp_i64 user_id, bool return_to_game_btn) const;

        void set_user_money(			const GameId& game_id, gbp_i64 user_id, gbp_i64 money) const;
        void set_chat_message(			const GameId& game_id, gbp_i64 user_id, const std::string &msg) const;
        void show_user_hand(			const GameId& game_id, gbp_i64 user_id, const std::vector<gbp_i8> &cards) const;
        void ask_blind(					const GameId& game_id, bool is_big_blind, gbp_i64 blind) const;
        void from_player_to_viewer(		const GameId& game_id, bool distributions_reason, bool recharge_reason) const;
        void set_current_user(			const GameId& game_id, gbp_i64 current_user_id, time_s time_left) const;
        void add_table_cards(			const GameId& game_id, const std::vector<gbp_i8> &table_cards) const;
        void add_user(					const GameId& game_id, const ns_table::table_player_info_t &user_info) const;
        void remove_users(				const GameId& game_id, const std::vector<gbp_i64> &removed_users) const;
        void add_cards(					const GameId& game_id, const std::vector<std::pair<gbp_i64, gbp_i8>> &users_cards) const;
        void add_dealer_cards(			const GameId& game_id, const std::vector<std::pair<gbp_i64, gbp_i8>> &users_cards) const;
        void set_pots(					const GameId& game_id, const std::vector<gbp_i64> &pots) const;
        void set_predefined_actions(	const GameId& game_id, const ns_table::money_info_t &money_info) const;
        void set_user_action(			const GameId& game_id, const ns_table::user_action_info_t &info) const;
        void set_user_actions(			const GameId& game_id, const std::vector<ns_table::available_user_course_t> &actions, const ns_table::action_info_t &action_info) const;
        void set_users_cashes(			const GameId& game_id, const ns_table::users_cashes_t &users_cashes) const;
        void set_winners(				const GameId& game_id, const std::pair<std::vector<ns_table::winner_info_t>, std::vector<ns_table::winner_info_t>>& winners
                                                             , const std::vector<std::pair<gbp_i64, std::vector<gbp_i8>>> &users_cards, bool by_combination) const;
//---------------- tournament signals
        void show_tournament_dialog_info(gbp_i64 tournament_id, const ns_table::tournament_dialog_info &info) const;
        void tournament_canceled(        gbp_i64 tournament_id, gbp_i64 buy_in) const;

        void hide_addon(                  const GameId& game_id) const;
        void hide_rebuy_btn(              const GameId& game_id) const;
        void game_deleted(                const GameId& game_id) const;
        void show_rebuy_buttons(          const GameId& game_id, bool double_available) const;

        void show_autobreak_massage(      const GameId& game_id, time_s delay_time) const;
        void show_autobreak_addon_massage(const GameId& game_id, time_s delay_time) const;
        void tournament_break(            const GameId& game_id, time_s break_time) const;
        void tournament_addon_break(      const GameId& game_id, time_s break_time, gbp_i64 addon_money, gbp_i64 addon_chips) const;

        void addon_available(             const GameId& game_id, const gbp::ns_api::addon_t &info, gbp_i64 account_money, time_s time_left) const;
        void bounty(                      const GameId& game_id, const std::vector<std::string> &nicknames, gbp_i64 money) const;
        void open_tournament(             const GameId& game_id, const ns_table::tournament_state_t &tournament_state, const ns_table::table_state_t &table_state, gbp_i64 players_count) const;
        void level_up(                    const GameId& game_id, gbp_i64 big_blind, gbp_i64 ante, qint16 current_round) const;
        void rebuys_waiting(              const GameId& game_id, time_s waiting_time) const;
                                          // new_game_id, new_pos должны все равно быть в table_state
        void table_change(                const GameId& old_game_id, const ns_table::table_state_t &table_state) const;
    };

    struct table_cash : gbp::ns_api::ns_table::ns_server_request::cash
    {
        using emitter_t = emitter_table;
    private:
        std::shared_ptr<gbp::net::object_registry> m_reg;
        NetworkClient& m_client;
        bool m_isInitialized;
        DLog *m_dLog;
    protected:
        DLog& log(gbp_i64 gameId = -1, gbp_i64 tournamentId = -1);
    public:
        explicit table_cash(NetworkClient * const client);

        const emitter_t *emitter() const;
        void init();

        static const std::string& parentClassName();
        // --------------------------------------------------------------
        virtual void set_table_state(gbp_i64 game_id, const ns_table::table_state_t &table_state) override;
        virtual void show_guest_buttons(gbp_i64 game_id) override;
        virtual void hide_guest_buttons(gbp_i64 game_id) override;
        virtual void show_hand_buttons(gbp_i64 tournament_id, gbp_i64 game_id) override;
        virtual void show_user_hand(gbp_i64 tournament_id, gbp_i64 game_id, gbp_i64 user_id, const std::vector<gbp_i8> &cards) override;
        //! @brief requested when current player sits on table from waiter
        virtual void open_table(gbp_i64 game_id, const ns_table::sit_on_table_info_t &info, const ns_table::table_state_t &table_state) override;
        virtual void open_cashes(const std::vector<ns_table::table_state_t> &cashes) override;
        virtual void close_table(gbp_i64 tournament_id, gbp_i64 game_id) override;
        virtual void set_user_actions(gbp_i64 tournament_id, gbp_i64 game_id, const std::vector<ns_table::available_user_course_t> &actions, const ns_table::action_info_t &action_info) override;
        virtual void set_user_action(gbp_i64 tournament_id, gbp_i64 game_id, const ns_table::user_action_info_t &info) override;
        virtual void set_common_pot(gbp_i64 tournament_id, gbp_i64 game_id, gbp_i64 pot) override;
        virtual void clear_common_pot(gbp_i64 tournament_id, gbp_i64 game_id) override;
        virtual void set_predefined_actions(gbp_i64 tournament_id, gbp_i64 game_id, const ns_table::money_info_t &money_info) override;
        virtual void add_cards(gbp_i64 tournament_id, gbp_i64 game_id, const std::vector<std::pair<gbp_i64, gbp_i8>> &users_cards) override;
        virtual void add_dealer_cards(gbp_i64 tournament_id, gbp_i64 game_id, const std::vector<std::pair<gbp_i64, gbp_i8>> &users_cards) override;
        virtual void add_table_cards(gbp_i64 tournament_id, gbp_i64 game_id, const std::vector<gbp_i8> &table_cards) override;
        virtual void set_chat_message(gbp_i64 tournament_id, gbp_i64 game_id, gbp_i64 user_id, const std::string &msg) override;
        virtual void ask_blind(gbp_i64 tournament_id, gbp_i64 game_id, bool is_big_blind, gbp_i64 blind) override;
        virtual void freeze_user(gbp_i64 tournament_id, gbp_i64 game_id, gbp_i64 user_id) override;
        virtual void put_user_in_anticipant(gbp_i64 tournament_id, gbp_i64 game_id, gbp_i64 user_id, bool return_to_game_btn) override;
        virtual void add_user(gbp_i64 tournament_id, gbp_i64 game_id, const ns_table::table_player_info_t &user_info) override;
        virtual void remove_users(gbp_i64 tournament_id, gbp_i64 game_id, const std::vector<gbp_i64> &removed_users) override;
        virtual void from_player_to_viewer(gbp_i64 tournament_id, gbp_i64 game_id, bool distributions_reason, bool recharge_reason) override;
        virtual void set_winners(gbp_i64 tournament_id, gbp_i64 game_id, const std::pair<std::vector<ns_table::winner_info_t>, std::vector<ns_table::winner_info_t>>& winners
                                                                       , const std::vector<std::pair<gbp_i64, std::vector<gbp_i8>>> &users_cards, bool by_combination) override;
        virtual void set_users_cashes(gbp_i64 tournament_id, gbp_i64 game_id, const ns_table::users_cashes_t &users_cashes) override;
        virtual void set_current_user(gbp_i64 tournament_id, gbp_i64 game_id, gbp_i64 current_user_id, time_s time_left) override;
        virtual void set_pots(gbp_i64 tournament_id, gbp_i64 game_id, const std::vector<gbp_i64> &pots) override;
        virtual void set_dealer(gbp_i64 tournament_id, gbp_i64 game_id, gbp_i64 user_id) override;
        virtual void clear_cards(gbp_i64 tournament_id, gbp_i64 game_id) override;

        virtual void set_distribution_number(gbp_i64 tournament_id, gbp_i64 game_id, gbp_i64 distribution_number) override;
        virtual void set_current_round_type(gbp_i64 tournament_id, gbp_i64 game_id, gbp::ns_api::ns_table::e_game_round_type round_type) override;
        virtual void end_of_trade_round(gbp_i64 tournament_id, gbp_i64 game_id, gbp_u8 round_number) override;
        virtual void end_of_distribution(gbp_i64 tournament_id, gbp_i64 game_id) override;
        virtual void end_of_game(gbp_i64 tournament_id, gbp_i64 game_id) override;

        virtual void set_user_online(gbp_i64 tournament_id, gbp_i64 game_id, gbp_i64 user_id) override;
        virtual void set_new_waiting_position(gbp_i64 game_id, uint waiters_count, quint16 w_pos) override;
        virtual void show_recharge_message(gbp_i64 game_id, const ns_table::recharge_money_t &money) override;
        virtual void set_user_money(gbp_i64 tournament_id, gbp_i64 game_id, gbp_i64 user_id, gbp_i64 money_) override;
    };

    struct table_tournament : gbp::ns_api::ns_table::ns_server_request::tournament
    {
        using emitter_t = emitter_table;
    private:
        std::shared_ptr<gbp::net::object_registry> m_reg;
        NetworkClient& m_client;
        bool m_isInitialized;
        DLog *m_dLog;
    public:
        explicit table_tournament(NetworkClient * const client);

        const emitter_t* emitter() const;
        void init();
        DLog& log(gbp_i64 gameId = -1, gbp_i64 tournamentId = -1);

        static const std::string& parentClassName();
        // --------------------------------------------------------------
        virtual void tournament_break(gbp_i64 tournament_id, gbp_i64 game_id, time_s break_time) override;
        virtual void tournament_addon_break(gbp_i64 tournament_id, gbp_i64 game_id, time_s break_time, gbp_i64 addon_money, gbp_i64 addon_chips) override;
        virtual void show_autobreak_massage(gbp_i64 tournament_id, gbp_i64 game_id, time_s delay_time) override;
        virtual void show_autobreak_addon_massage(gbp_i64 tournament_id, gbp_i64 game_id, time_s delay_time) override;
        virtual void addon_available(gbp_i64 tournament_id, gbp_i64 game_id, const gbp::ns_api::addon_t &info, gbp_i64 account_money, time_s time_left) override;
        virtual void bounty(gbp_i64 tournament_id, gbp_i64 game_id, const std::vector<std::string> &nicknames, gbp_i64 money) override;
        virtual void open_tournament(gbp_i64 tournament_id, gbp_i64 game_id, const ns_table::tournament_state_t &tournament_state, const ns_table::table_state_t &table_state, gbp_i64 players_count) override;
        virtual void table_change(gbp_i64 tournament_id, gbp_i64 old_game_id, gbp_i64 new_game_id, gbp_i8 new_pos, const ns_table::table_state_t &table_state) override;
        virtual void show_rebuy_buttons(gbp_i64 tournament_id, gbp_i64 game_id, bool double_available) override;
        virtual void hide_addon(gbp_i64 tournament_id, gbp_i64 game_id) override;
        virtual void hide_rebuy_btn(gbp_i64 tournament_id, gbp_i64 game_id) override;
        virtual void show_tournament_dialog_info(gbp_i64 tournament_id, const ns_table::tournament_dialog_info &info) override;
        virtual void level_up(gbp_i64 tournament_id, gbp_i64 game_id, gbp_i64 big_blind, gbp_i64 ante, qint16 current_round) override;
        virtual void tournament_canceled(gbp_i64 tournament_id, gbp_i64 buy_in) override;
        virtual void rebuys_waiting(gbp_i64 tournament_id, gbp_i64 game_id, time_s waiting_time) override;
        virtual void game_deleted(gbp_i64 tournament_id, gbp_i64 game_id) override;
    };
} // namespace ns_server_request

// std::vector<gbp_i8> ~ cards
//Q_DECLARE_METATYPE(ns_table::cards)
//Q_DECLARE_METATYPE(ns_table::table_player_info_t)
//Q_DECLARE_METATYPE(ns_table::table_state_t)
