#include <precompiled.h>
#include "server_request.h"
#include "../network_client.h"
#include <iostream>
#include <core/money.h>
#include <basewidgets/types.h>
#include <qfile.h>
#include <qdir.h>

#define INFO(name, __info)                                          \
qDebug() << QString(">>--%0").arg(name) << #__info;                 \
std::cout << __info << std::endl;                                   \
qDebug() << ">>--------------------------------------------------"

namespace
{
    using time_s = gbp::ns_api::time_s;
}


class DLog
{
private:
    QFile m_file;
    QMap<GamePlayerId, QFile*> m_gameLogs;

    QFile* m_currentLog;
    QDateTime m_lastMark;
public:
    DLog(const char* path)
        : m_file(path)
        , m_currentLog(&m_file)
    {
        bool isOpen = m_currentLog->isOpen() || m_currentLog->open(QIODevice::Append | QIODevice::Text);
        if (isOpen) {
            m_currentLog->write(QString("\n New session\n%0\n").arg(QDateTime::currentDateTime().toString("hh:mm:ss dd.MM")).toLatin1());
            m_currentLog->close();
        }
    }
    ~DLog() {
        qDeleteAll(m_gameLogs);
    }

    template <typename T, typename ...Args>
    void add(T arg, Args... args)
    {
        m_currentLog->open(QIODevice::Append | QIODevice::Text);
        std::ostringstream os;
        toOs(os, arg);
        m_currentLog->write(os.str().c_str());
        m_currentLog->write("\n");
        m_currentLog->close();
        add(args...);
    }
    template <typename T>
    void add(T arg)
    {
        m_currentLog->open(QIODevice::Append | QIODevice::Text);
        std::ostringstream os;
        toOs(os, arg);
        if (m_lastMark != QDateTime::currentDateTime()) {
            m_lastMark = QDateTime::currentDateTime();
            m_currentLog->write(m_lastMark.toString("****** mm:ss        ").toLatin1());
        } else {
            m_currentLog->write("[--:--]        ");
        }
        m_currentLog->write(os.str().c_str());
        m_currentLog->write("\n");
        m_currentLog->close();
    }

    void add(void)
    {
        if ((m_currentLog->isOpen() && m_currentLog->openMode() & QIODevice::Append) || m_currentLog->open(QIODevice::Append | QIODevice::Text)) {
	        m_currentLog->write("done\n");
	        m_currentLog->close();
	    } else {
            qWarning() << "Can not open the device" << QString("%2 %0:%1").arg(__FILE__).arg(__LINE__).arg(__FUNCTION__);
	    }
    }

    template <typename T>
    void toOs(std::ostringstream& os, const T& value) {
        os << value;
    }

    DLog& game(GameId gameID) {
        GamePlayerId id(gameID, netClient().userId());
        if (id.m_gameID.m_tableID == -1) {
            m_currentLog = &m_file;
        } else {
            QMap<GamePlayerId, QFile*>::iterator it = m_gameLogs.find(id);
            if (it == m_gameLogs.end()) {
                QDir::current().mkpath("../logs/server_request/");
                it = m_gameLogs.insert(id, new QFile(QString("../logs/server_request/gameLOG_") + id.repr() + ".dlog"));
            }
            m_currentLog = it.value();
        }
        return *this;
    }

    template <typename T>
    DLog& operator<<(const T& arg) {
        add(arg);
        return *this;
    }
};

template <>
void DLog::toOs(std::ostringstream& os, const QString& value) {
    os << value.toStdString();
}

namespace ns_server_request
{

    /*static*/ const std::string &table_cash::parentClassName()

    {
        static const std::string className = "gbp::ns_api::ns_table::ns_server_request::cash";
        return className;
    }

    DLog &table_cash::log(gbp_i64 gameId, gbp_i64 tournamentId)
    {
        return m_dLog->game(GameId(gameId, tournamentId));
    }

    table_cash::table_cash(NetworkClient* const client)
        : gbp::ns_api::ns_table::ns_server_request::cash()
        , m_reg(client->create_registry(parentClassName()))
        , m_client(*client)
        , m_isInitialized(false)
    {
        init();

        m_dLog = new DLog("serverRequests.dlog");
    }

    const emitter_table *table_cash::emitter() const {
        static const emitter_t e;
        return &e;
    }

    void table_cash::init()
    {
        if (!m_isInitialized)
        {
            m_reg->on("set_table_state",          this, &table_cash::set_table_state);
            m_reg->on("show_guest_buttons",       this, &table_cash::show_guest_buttons);
            m_reg->on("hide_guest_buttons",       this, &table_cash::hide_guest_buttons);
            m_reg->on("show_hand_buttons",        this, &table_cash::show_hand_buttons);
            m_reg->on("show_user_hand",           this, &table_cash::show_user_hand);
            m_reg->on("open_table",               this, &table_cash::open_table);
            m_reg->on("open_cashes",              this, &table_cash::open_cashes);
            m_reg->on("close_table",              this, &table_cash::close_table);
            m_reg->on("set_user_actions",         this, &table_cash::set_user_actions);
            m_reg->on("set_user_action",          this, &table_cash::set_user_action);
            m_reg->on("set_common_pot",           this, &table_cash::set_common_pot);
            m_reg->on("clear_common_pot",         this, &table_cash::clear_common_pot);
            m_reg->on("set_predefined_actions",   this, &table_cash::set_predefined_actions);
            m_reg->on("add_cards",                this, &table_cash::add_cards);
            m_reg->on("add_dealer_cards",         this, &table_cash::add_dealer_cards);
            m_reg->on("set_distribution_number",  this, &table_cash::set_distribution_number);
            m_reg->on("set_current_round_type",   this, &table_cash::set_current_round_type);
            m_reg->on("add_table_cards",          this, &table_cash::add_table_cards);
            m_reg->on("set_chat_message",         this, &table_cash::set_chat_message);
            m_reg->on("ask_blind",                this, &table_cash::ask_blind);
            m_reg->on("freeze_user",              this, &table_cash::freeze_user);
            m_reg->on("put_user_in_anticipant",   this, &table_cash::put_user_in_anticipant);
            m_reg->on("add_user",                 this, &table_cash::add_user);
            m_reg->on("remove_users",             this, &table_cash::remove_users);
            m_reg->on("from_player_to_viewer",    this, &table_cash::from_player_to_viewer);
            m_reg->on("set_winners",              this, &table_cash::set_winners);
            m_reg->on("set_users_cashes",         this, &table_cash::set_users_cashes);
            m_reg->on("set_current_user",         this, &table_cash::set_current_user);
            m_reg->on("set_pots",                 this, &table_cash::set_pots);
            m_reg->on("set_dealer",               this, &table_cash::set_dealer);
            m_reg->on("clear_cards",              this, &table_cash::clear_cards);
            m_reg->on("end_of_trade_round",       this, &table_cash::end_of_trade_round);
            m_reg->on("end_of_distribution",      this, &table_cash::end_of_distribution);
            m_reg->on("end_of_game",              this, &table_cash::end_of_game);
            m_reg->on("set_user_online",          this, &table_cash::set_user_online);
            m_reg->on("set_new_waiting_position", this, &table_cash::set_new_waiting_position);
            m_reg->on("show_recharge_message",    this, &table_cash::show_recharge_message);
            m_reg->on("set_user_money",           this, &table_cash::set_user_money);
            m_isInitialized = true;
        }
    }

    void table_cash::set_table_state(gbp_i64 game_id, const gbp::ns_api::ns_table::table_state_t &table_state)
    {
        log(game_id) << "table_cash::set_table_state()" << table_state;
    	emit emitter()->set_table_state(game_id, table_state);
    }

    void table_cash::show_guest_buttons(gbp_i64 game_id)
    {
        log(game_id) << QString("table_cash::show_guest_buttons()");
        emit emitter()->show_guest_buttons(game_id, true);
    }

    void table_cash::hide_guest_buttons(gbp_i64 game_id)
    {
        log(game_id) << QString("table_cash::hide_guest_buttons()");
        emit emitter()->show_guest_buttons(game_id, false);
    }

    void table_cash::show_hand_buttons(gbp_i64 tournament_id, gbp_i64 game_id)
    {
        log(game_id, tournament_id) << QString("table_cash::show_hand_buttons()");
        GameId id(game_id, tournament_id);
        emit emitter()->show_hand_buttons(id);
    }

    void table_cash::show_user_hand(gbp_i64 tournament_id, gbp_i64 game_id, gbp_i64 user_id, const std::vector<gbp_i8> &cards)
    {
    	log(game_id, tournament_id) << QString("table_cash::show_user_hand(user_id: %0, cards:").arg(user_id) << cards;
        GameId id(game_id, tournament_id);
    	emit emitter()->show_user_hand(id, user_id, cards);
    }

    void table_cash::open_table(gbp_i64 game_id, const gbp::ns_api::ns_table::sit_on_table_info_t &info, const gbp::ns_api::ns_table::table_state_t &table_state)
    {
        log(game_id) << QString("table_cash::open_table(%0)").arg(game_id).toLatin1().data() << table_state << info;
    	emit emitter()->open_table(game_id, info, table_state);
    }

    void table_cash::open_cashes(const std::vector<gbp::ns_api::ns_table::table_state_t> &cashes)
    {
        log() << QString("open_cashes") << cashes;
    	emit emitter()->open_cashes(cashes);
    }

    void table_cash::close_table(gbp_i64 tournament_id, gbp_i64 game_id)
    {
        log(game_id, tournament_id) << QString("table_cash::close_table()");
        GameId id(game_id, tournament_id);
        std::cout << "server_request " << id.toString().toStdString() << std::endl;
        emit emitter()->close_table(id);
    }

    void table_cash::set_user_actions(gbp_i64 tournament_id, gbp_i64 game_id, const std::vector<gbp::ns_api::ns_table::available_user_course_t> &actions, const gbp::ns_api::ns_table::action_info_t &action_info)
    {
        log(game_id, tournament_id) << QString("table_cash::set_user_actions()") << action_info << actions;
        GameId id(game_id, tournament_id);
    	emit emitter()->set_user_actions(id, actions, action_info);
    }

    void table_cash::set_user_action(gbp_i64 tournament_id, gbp_i64 game_id, const gbp::ns_api::ns_table::user_action_info_t &info)
    {
    	log(game_id, tournament_id) << QString("table_cash::set_user_action()") << info;
        GameId id(game_id, tournament_id);
    	emit emitter()->set_user_action(id, info);
    }

    void table_cash::set_common_pot(gbp_i64 tournament_id, gbp_i64 game_id, gbp_i64 pot)
    {
    	log(game_id, tournament_id) << QString("table_cash::set_common_pot(%0)").arg(Money(pot).toString());
        GameId id(game_id, tournament_id);
    	emit emitter()->set_common_pot(id, pot);
    }

    void table_cash::clear_common_pot(gbp_i64 tournament_id, gbp_i64 game_id)
    {
    	log(game_id, tournament_id) << QString("table_cash::clear_common_pot()");
        GameId id(game_id, tournament_id);
        emit emitter()->clear_common_pot(id);
    }

    void table_cash::set_predefined_actions(gbp_i64 tournament_id, gbp_i64 game_id, const gbp::ns_api::ns_table::money_info_t &money_info)
    {
        log(game_id, tournament_id) << QString("table_cash::set_predefined_actions()") << money_info;
        GameId id(game_id, tournament_id);
    	emit emitter()->set_predefined_actions(id, money_info);
    }

    void table_cash::add_cards(gbp_i64 tournament_id, gbp_i64 game_id, const std::vector<std::pair<gbp_i64, gbp_i8>> &users_cards)
    {
    	log(game_id, tournament_id) << QString("table_cash::add_cards()") << users_cards;
        GameId id(game_id, tournament_id);
    	emit emitter()->add_cards(id, users_cards);
    }

    void table_cash::add_dealer_cards(gbp_i64 tournament_id, gbp_i64 game_id, const std::vector<std::pair<gbp_i64, gbp_i8>> &users_cards)
    {
    	log(game_id, tournament_id) << QString("table_cash::add_dealer_cards()") << users_cards;
        GameId id(game_id, tournament_id);
    	emit emitter()->add_dealer_cards(id, users_cards);
    }

    void table_cash::set_distribution_number(gbp_i64 tournament_id, gbp_i64 game_id, gbp_i64 distribution_number)
    {
    	log(game_id, tournament_id) << QString("table_cash::set_distribution_number(distribution_number: %0)").arg(distribution_number);
        GameId id(game_id, tournament_id);
    	emit emitter()->set_distribution_number(id, distribution_number);
    }

    void table_cash::set_current_round_type(gbp_i64 tournament_id, gbp_i64 game_id, gbp::ns_api::ns_table::e_game_round_type round_type)
    {
        log(game_id, tournament_id) << QString("table_cash::set_current_round_type(%0)").arg(ns_util::toString(round_type));
        GameId id(game_id, tournament_id);
    	emit emitter()->set_current_round_type(id, round_type);
    }

    void table_cash::add_table_cards(gbp_i64 tournament_id, gbp_i64 game_id, const std::vector<gbp_i8> &table_cards)
    {
    	log(game_id, tournament_id) << QString("table_cash::add_table_cards()") << table_cards;
        GameId id(game_id, tournament_id);
    	emit emitter()->add_table_cards(id, table_cards);
    }

    void table_cash::set_chat_message(gbp_i64 tournament_id, gbp_i64 game_id, gbp_i64 user_id, const std::string &msg)
    {
        log(game_id, tournament_id) << QString("table_cash::set_chat_message(user_id: %0)").arg(user_id) << msg;
        GameId id(game_id, tournament_id);
    	emit emitter()->set_chat_message(id, user_id, msg);
    }

    void table_cash::ask_blind(gbp_i64 tournament_id, gbp_i64 game_id, bool is_big_blind, gbp_i64 blind)
    {
    	log(game_id, tournament_id) << QString("table_cash::ask_blind(blind: %0, is_big_blind: %1)").arg(Money(blind).toString()).arg(is_big_blind);
        GameId id(game_id, tournament_id);
    	emit emitter()->ask_blind(id, is_big_blind, blind);
    }

    void table_cash::freeze_user(gbp_i64 tournament_id, gbp_i64 game_id, gbp_i64 user_id)
    {
    	log(game_id, tournament_id) << QString("table_cash::freeze_user(user_id: %0)").arg(user_id);
        GameId id(game_id, tournament_id);
    	emit emitter()->freeze_user(id, user_id);
    }

    void table_cash::put_user_in_anticipant(gbp_i64 tournament_id, gbp_i64 game_id, gbp_i64 user_id, bool return_to_game_btn)
    {
    	log(game_id, tournament_id) << QString("table_cash::put_user_in_anticipant(userID: %0, bttnShow: %1)").arg(user_id).arg(return_to_game_btn);
        GameId id(game_id, tournament_id);
    	emit emitter()->put_user_in_anticipant(id, user_id, return_to_game_btn);
    }

    void table_cash::add_user(gbp_i64 tournament_id, gbp_i64 game_id, const gbp::ns_api::ns_table::table_player_info_t &user_info)
    {
    	log(game_id, tournament_id) << QString("table_cash::add_user(tournament_id)") << user_info;
        GameId id(game_id, tournament_id);
    	emit emitter()->add_user(id, user_info);
    }

    void table_cash::remove_users(gbp_i64 tournament_id, gbp_i64 game_id, const std::vector<gbp_i64> &removed_users)
    {
    	log(game_id, tournament_id) << QString("table_cash::remove_users") << removed_users;
        GameId id(game_id, tournament_id);
    	emit emitter()->remove_users(id, removed_users);
    }

    void table_cash::from_player_to_viewer(gbp_i64 tournament_id, gbp_i64 game_id, bool distributions_reason, bool recharge_reason)
    {
    	log(game_id, tournament_id) << QString("table_cash::from_player_to_viewer(distributions_reason: %0, recharge_reason: %1)").arg(distributions_reason).arg(recharge_reason);
        GameId id(game_id, tournament_id);
        qDebug() << "from_player_to_viewer";
    	emit emitter()->from_player_to_viewer(id, distributions_reason, recharge_reason);
    }

    void table_cash::set_winners(gbp_i64 tournament_id, gbp_i64 game_id, const gbp::ns_api::ns_table::hi_low_winners &winners, const gbp::ns_api::ns_table::users_and_cards &users_cards, bool by_combination)
    {
    	log(game_id, tournament_id) << QString("table_cash::set_winners(by_combination: %0)").arg(by_combination) << winners << users_cards;
        GameId id(game_id, tournament_id);
    	emit emitter()->set_winners(id, winners, users_cards, by_combination);
    }

    void table_cash::set_users_cashes(gbp_i64 tournament_id, gbp_i64 game_id, const gbp::ns_api::ns_table::users_cashes_t &users_cashes)
    {
    	log(game_id, tournament_id) << QString("table_cash::set_users_cashes") << users_cashes;
        GameId id(game_id, tournament_id);
    	emit emitter()->set_users_cashes(id, users_cashes);
    }

    void table_cash::set_current_user(gbp_i64 tournament_id, gbp_i64 game_id, gbp_i64 current_user_id, time_s time_left)
    {
        log(game_id, tournament_id) << QString("table_cash::set_current_user(user: %0, time_left: %1)").arg(current_user_id).arg(time_left);
        GameId id(game_id, tournament_id);
        emit emitter()->set_current_user(id, current_user_id, time_left);
    }

    void table_cash::set_pots(gbp_i64 tournament_id, gbp_i64 game_id, const std::vector<gbp_i64> &pots)
    {
        QString potsStr = Money(pots.at(0)).toString();
        for (auto it = pots.begin()+1; it != pots.end(); ++it) {
            potsStr += ", "; potsStr += Money(*it).toString();
        }
        log(game_id, tournament_id) << QString("table_cash::set_pots(%0)").arg(potsStr);
        GameId id(game_id, tournament_id);
    	emit emitter()->set_pots(id, pots);
    }

    void table_cash::set_dealer(gbp_i64 tournament_id, gbp_i64 game_id, gbp_i64 user_id)
    {
        log(game_id, tournament_id) << QString("table_cash::set_dealer(user_id: %0)").arg(user_id) << user_id;
        GameId id(game_id, tournament_id);
    	emit emitter()->set_dealer(id, user_id);
    }

    void table_cash::clear_cards(gbp_i64 tournament_id, gbp_i64 game_id)
    {
        log(game_id, tournament_id) << QString("table_cash::clear_cards()");
        GameId id(game_id, tournament_id);
        emit emitter()->clear_cards(id);
    }

    void table_cash::end_of_trade_round(gbp_i64 tournament_id, gbp_i64 game_id, gbp_u8 round_number)
    {
        log(game_id, tournament_id) << QString("table_cash::end_of_trade_round(round_number: %0)").arg(round_number) << round_number;
        GameId id(game_id, tournament_id);
        emit emitter()->end_of_trade_round(id, round_number);
    }

    void table_cash::end_of_distribution(gbp_i64 tournament_id, gbp_i64 game_id)
    {
        log(game_id, tournament_id) << QString("table_cash::end_of_distribution()");
        GameId id(game_id, tournament_id);
    	emit emitter()->end_of_distribution(id);
    }

    void table_cash::end_of_game(gbp_i64 tournament_id, gbp_i64 game_id)
    {
        log(game_id, tournament_id) << QString("table_cash::end_of_game()");
        GameId id(game_id, tournament_id);
    	emit emitter()->end_of_game(id);
    }

    void table_cash::set_user_online(gbp_i64 tournament_id, gbp_i64 game_id, gbp_i64 user_id)
    {
        log(game_id, tournament_id) << QString("table_cash::set_user_online(user_id %0)").arg(user_id) << user_id;
        GameId id(game_id, tournament_id);
    	emit emitter()->set_user_online(id, user_id);
    }

    void table_cash::set_new_waiting_position(gbp_i64 game_id, uint waiters_count, quint16 w_pos)
    {
        log(game_id) << QString("table_cash::set_new_waiting_position(waiters_count: %0, waiter_pos: %1)").arg(waiters_count).arg(w_pos);
    	emit emitter()->set_new_waiting_position(game_id, waiters_count, w_pos);
    }

    void table_cash::show_recharge_message(gbp_i64 game_id, const gbp::ns_api::ns_table::recharge_money_t &money)
    {
        log(game_id) << QString("table_cash::show_recharge_message()") << money;
    	emit emitter()->show_recharge_message(game_id, money);
    }

    void table_cash::set_user_money(gbp_i64 tournament_id, gbp_i64 game_id, gbp_i64 user_id, gbp_i64 money)
    {
        if (tournament_id == -1) {
           log(game_id) << QString("table_cash::set_user_money(user_id: %0, money: %1)").arg(user_id).arg(Money(money).toString());
        } else {
           log(game_id, tournament_id) << QString("table_cash::set_user_money(tournament_id: %0, game_id: %1, user_id: %2, money: %3)").arg(tournament_id).arg(game_id).arg(user_id).arg(money);
        }
        GameId id(game_id, tournament_id);
        emit emitter()->set_user_money(id, user_id, money);
    }
    //--------------------------------------------- ---------- ---------------------------------------//
    //--------------------------------------------- TOURNAMENT ---------------------------------------//
    //--------------------------------------------- ---------- ---------------------------------------//


    /*static*/ const std::string &table_tournament::parentClassName()

    {
        static const std::string className = "gbp::ns_api::ns_table::ns_server_request::tournament";
        return className;
    }

    DLog &table_tournament::log(gbp_i64 gameId, gbp_i64 tournamentId)
    {
        return m_dLog->game(GameId(gameId, tournamentId));
    }

    table_tournament::table_tournament(NetworkClient* const client)
        : gbp::ns_api::ns_table::ns_server_request::tournament()
        , m_reg(client->create_registry(parentClassName()))
        , m_client(*client)
        , m_isInitialized(false)
    {
        init();

        m_dLog = new DLog("tournamentServerRequests.dlog");
    }

    const emitter_table *table_tournament::emitter() const {
        static const emitter_t e;
        return &e;
    }

    void table_tournament::init()
    {
        if (!m_isInitialized)
        {
            m_reg->on("tournament_break",              this, &table_tournament::tournament_break);
            m_reg->on("tournament_addon_break",        this, &table_tournament::tournament_addon_break);
            m_reg->on("show_autobreak_massage",        this, &table_tournament::show_autobreak_massage);
            m_reg->on("show_autobreak_addon_massage",  this, &table_tournament::show_autobreak_addon_massage);
            m_reg->on("addon_available",               this, &table_tournament::addon_available);
            m_reg->on("bounty",                        this, &table_tournament::bounty);
            m_reg->on("open_tournament",               this, &table_tournament::open_tournament);
            m_reg->on("table_change",                  this, &table_tournament::table_change);
            m_reg->on("show_rebuy_buttons",            this, &table_tournament::show_rebuy_buttons);
            m_reg->on("hide_addon",                    this, &table_tournament::hide_addon);
            m_reg->on("hide_rebuy_btn",                this, &table_tournament::hide_rebuy_btn);
            m_reg->on("show_tournament_dialog_info",   this, &table_tournament::show_tournament_dialog_info);
            m_reg->on("level_up",                      this, &table_tournament::level_up);
            m_reg->on("tournament_canceled",           this, &table_tournament::tournament_canceled);
            m_reg->on("rebuys_waiting",                this, &table_tournament::rebuys_waiting);
            m_reg->on("game_deleted",                  this, &table_tournament::game_deleted);
            m_isInitialized = true;
        }
    }

    void table_tournament::tournament_break(gbp_i64 tournament_id, gbp_i64 game_id, time_s break_time) {
        GameId gameID(game_id, tournament_id);
        emit emitter()->tournament_break(gameID, break_time);
    }

    void table_tournament::tournament_addon_break(gbp_i64 tournament_id, gbp_i64 game_id, time_s break_time, gbp_i64 addon_money, gbp_i64 addon_chips) {
        GameId gameID(game_id, tournament_id);
        emit emitter()->tournament_addon_break(gameID, break_time, addon_money, addon_chips);
    }

    void table_tournament::show_autobreak_massage(gbp_i64 tournament_id, gbp_i64 game_id, time_s delay_time) {
        GameId gameID(game_id, tournament_id);
        emit emitter()->show_autobreak_massage(gameID, delay_time);
    }

    void table_tournament::show_autobreak_addon_massage(gbp_i64 tournament_id, gbp_i64 game_id, time_s delay_time) {
        GameId gameID(game_id, tournament_id);
        emit emitter()->show_autobreak_addon_massage(gameID, delay_time);
    }

    void table_tournament::addon_available(gbp_i64 tournament_id, gbp_i64 game_id, const gbp::ns_api::addon_t &info, gbp_i64 account_money, time_s time_left) {
        GameId gameID(game_id, tournament_id);
        emit emitter()->addon_available(gameID, info, account_money, time_left);
    }

    void table_tournament::bounty(gbp_i64 tournament_id, gbp_i64 game_id, const std::vector<std::string> &nicknames, gbp_i64 money) {
        GameId gameID(game_id, tournament_id);
        emit emitter()->bounty(gameID, nicknames, money);
    }

    void table_tournament::open_tournament(gbp_i64 tournament_id, gbp_i64 game_id, const gbp::ns_api::ns_table::tournament_state_t &tournament_state, const gbp::ns_api::ns_table::table_state_t &table_state, gbp_i64 players_count) {
        GameId gameID(game_id, tournament_id);
        emit emitter()->open_tournament(gameID, tournament_state, table_state, players_count);
    }

    void table_tournament::table_change(gbp_i64 tournament_id, gbp_i64 old_game_id, gbp_i64 new_game_id, gbp_i8 new_pos, const ns_table::table_state_t &table_state) {
        GameId oldGameID(old_game_id, tournament_id);
        auto itCurrentUser = IdGetter<ns_table::table_player_info_t>::find_id(table_state._players, netClient().userId());
        Q_ASSERT(table_state._game_id == new_game_id && table_state._tournament_id == tournament_id && itCurrentUser != table_state._players.end() && itCurrentUser->_pos == new_pos);
        emit emitter()->table_change(oldGameID, table_state);
    }

    void table_tournament::show_rebuy_buttons(gbp_i64 tournament_id, gbp_i64 game_id, bool double_available) {
        GameId gameID(game_id, tournament_id);
        emit emitter()->show_rebuy_buttons(gameID, double_available);
    }

    void table_tournament::hide_addon(gbp_i64 tournament_id, gbp_i64 game_id) {
        GameId gameID(game_id, tournament_id);
        emit emitter()->hide_addon(gameID);
    }

    void table_tournament::hide_rebuy_btn(gbp_i64 tournament_id, gbp_i64 game_id) {
        GameId gameID(game_id, tournament_id);
        emit emitter()->hide_rebuy_btn(gameID);
    }

    void table_tournament::show_tournament_dialog_info(gbp_i64 tournament_id, const gbp::ns_api::ns_table::tournament_dialog_info &info) {
        emit emitter()->show_tournament_dialog_info(tournament_id, info);
    }

    void table_tournament::level_up(gbp_i64 tournament_id, gbp_i64 game_id, gbp_i64 big_blind, gbp_i64 ante, qint16 current_round) {
        GameId gameID(game_id, tournament_id);
        emit emitter()->level_up(gameID, big_blind, ante, current_round);
    }

    void table_tournament::tournament_canceled(gbp_i64 tournament_id, gbp_i64 buy_in) {
        emit emitter()->tournament_canceled(tournament_id, buy_in);
    }

    void table_tournament::rebuys_waiting(gbp_i64 tournament_id, gbp_i64 game_id, time_s waiting_time) {
        GameId gameID(game_id, tournament_id);
        emit emitter()->rebuys_waiting(gameID, waiting_time);
    }

    void table_tournament::game_deleted(gbp_i64 tournament_id, gbp_i64 game_id) {
        GameId gameID(game_id, tournament_id);
        emit emitter()->game_deleted(gameID);
    }
} //namespace ns_server_request


