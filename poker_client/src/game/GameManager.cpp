#include <precompiled.h>
#include "GameManager.h"
//#include "cashhandler.h"
#include <QtConcurrent/QtConcurrentRun>
#include <helpers/helper_statistic.h>
#include <QTimer>
#include <dialogs/buyin/dialog_CashBuyIn.h>
#ifndef Q_MOC_RUN
    #include <rpc/network_client.h>
    #include <rpc/table/reply.h>
    #include <rpc/rpc_common.h>
    #include <rpc/table/rpc_table.h>
    #include <rpc/table/server_request.h>
    #include <rpc/table/server_reply.h>
#endif //!Q_MOC_RUN
#include <dialogs/dialog_gametable.h>
#include <api_models/model_cash_stat_t.hpp>
#include <descriptors/descriptor_GameTable.h>
#include <game/actions/single_actions.h>

#include <db/SharedDataService.h>
#include <api_models/model_user_info_t.hpp>
#include <widgets/MainWindow.h>
#include "proxydatahandler.h"
#include "debuginfoprintergamehandler.h"
#include "proxyviewhandler.h"
#include "commongameviewhandler.h"
#include "controlshandler.h"
#include "tabledataupdater.h"

#include <widgets/widget_TableOver.h>


struct IdGetter<class server_request_handler_impl>;






namespace
{

} //namespace
template <typename Receiver, typename ...SlotArgs>
struct IdDropper {
    using ReceiverType = Receiver;
//    using SignalType   = std::function<void(Sender::*)(IDType, Args...)>;
//    template <typename ...Args> using AnySlotType = std::function<void(Receiver::*)(Args...)>;
    using SlotType = void(Receiver::*)(SlotArgs...);
    using SlotFn   = std::function<SlotType>;

    ReceiverType* receiver;
    SlotType slot;

    IdDropper(Receiver* receiver, SlotType slot)
        : receiver(receiver)
        , slot(slot)
    {}

    typename IdGetter<Receiver>::id_type receiverId() const {
        return get_id(receiver).copy();
    }

    template <typename T>
    bool matchId(T id) const {
        return receiverId() == id;
    }

    template <typename ...Args>
    void call_slot(void(Receiver::*fn)(Args...) const, Args... args) const {
        auto f = std::bind(fn, receiver);
        f(args...);
    }

    template <typename T>
    void checkDropCall(T id) const {
        if (matchId(id)) {
            [this]{ this->slot(); }();
        }
    }
    template <typename T>
    void checkDropCall(T id, SlotArgs ...args) const {
        if (matchId(id)) {
//            call_slot(slot, args...);
        }
    }
    template <typename T>
    void checkDropCall(T id, SlotArgs ...args) {
        if (matchId(id)) {
            call_slot(slot, args...);
        }
    }

    template <typename Sender, typename T, typename ...Args>
    QMetaObject::Connection connect(const Sender* sender, void(Sender::*signal)(T, Args...)const ) const {
        return QObject::connect(sender, signal, receiver, [&](T id, SlotArgs ...args){
            checkDropCall(id, args...);
        });
    }

};

template <typename ObjT, typename ...Args>
IdDropper<ObjT, Args...>
createIdDropper(ObjT* obj, void(ObjT::*f)(Args...))
{
    return IdDropper<ObjT, Args...>(obj, f);
}

class server_request_handler_impl : public QObject
{
    Q_OBJECT_FAKE
private:
    GameManager* m_mgr;
    const ns_server_request::emitter_table* m_emitter;
public:
    const GameId m_id;

    server_request_handler_impl(const GameId& id, const ns_server_request::emitter_table& emitter)
        : QObject(pApp->gameManager())
        , m_mgr(pApp->gameManager())
        , m_emitter(&emitter)
        , m_id(id)

    {
        connect(m_emitter, &ns_server_request::emitter_table::destroyed, this, &server_request_handler_impl::deleteLater);
        connect2(&ns_server_request::emitter_table::set_user_action, &server_request_handler_impl::set_user_action);
    }

public:
    template <typename Func1, typename Func2>
    QMetaObject::Connection connect2(Func1 signal, Func2 slot)
    {
        auto id_dropper = createIdDropper(this, slot);
        return id_dropper.connect(m_emitter, signal);
    }

public slots:
    void open_cashes(const std::vector<ns_table::table_state_t> &cashes) const {
        for (const ns_table::table_state_t& table_state: cashes) {
//            ActionOpenGameTable
        }
    }
    void show_guest_buttons(gbp_i64 game_id, bool) const {

    }
    void set_new_waiting_position(gbp_i64 game_id, uint waiters_count, quint16 w_pos) const {

    }
    void set_table_state(gbp_i64 game_id, const ns_table::table_state_t &table_state) const {

    }
    void show_recharge_message(gbp_i64 game_id, const ns_table::recharge_money_t &money) const {

    }
    void open_table(gbp_i64 game_id, const ns_table::sit_on_table_info_t &info, const ns_table::table_state_t &table_state) const {

    }
    void clear_cards(const GameId& game_id) const {

    }
    void clear_common_pot(const GameId& game_id) const {

    }
    void set_common_pot(const GameId& game_id, gbp_i64 pot) const {

    }
    void close_table(const GameId& game_id) const {

    }
    void set_distribution_number(const GameId& game_id, gbp_i64 distribution_number) const {

    }
    void end_of_distribution(const GameId& game_id) const {

    }
    void set_current_round_type(const GameId& game_id, gbp::ns_api::ns_table::e_game_round_type round_type) const {

    }
    void end_of_trade_round(const GameId& game_id, gbp_u8 round_number) const {

    }
    void end_of_game(const GameId& game_id) const {

    }
    void show_hand_buttons(const GameId& game_id) const {

    }
    void set_dealer(const GameId& game_id, gbp_i64 user_id) const {

    }
    void set_user_online(const GameId& game_id, gbp_i64 user_id) const {

    }
    void freeze_user(const GameId& game_id, gbp_i64 user_id) const {

    }
    void put_user_in_anticipant(const GameId& game_id, gbp_i64 user_id, bool return_to_game_btn) const {

    }
    void set_user_money(const GameId& game_id, gbp_i64 user_id, gbp_i64 money) const {

    }
    void set_chat_message(const GameId& game_id, gbp_i64 user_id, const std::string &msg) const {

    }
    void show_user_hand(const GameId& game_id, gbp_i64 user_id, const std::vector<gbp_i8> &cards) const {

    }
    void ask_blind(const GameId& game_id, bool is_big_blind, gbp_i64 blind) const {

    }
    void from_player_to_viewer(const GameId& game_id, bool distributions_reason, bool recharge_reason) const {

    }
    void set_current_user(const GameId& game_id, gbp_i64 current_user_id, time_s time_left) const {

    }
    void add_table_cards(const GameId& game_id, const std::vector<gbp_i8> &table_cards) const {

    }
    void add_user(const GameId& game_id, const ns_table::table_player_info_t &user_info) const {

    }
    void remove_users(const GameId& game_id, const std::vector<gbp_i64> &removed_users) const {

    }
    void add_cards(const GameId& game_id, const std::vector<std::pair<gbp_i64, gbp_i8>> &users_cards) const {

    }
    void add_dealer_cards(const GameId& game_id, const std::vector<std::pair<gbp_i64, gbp_i8>> &users_cards) const {

    }
    void set_pots(const GameId& game_id, const std::vector<gbp_i64> &pots) const {

    }
    void set_predefined_actions(const GameId& game_id, const ns_table::money_info_t &money_info) const {

    }
    void set_user_action(const ns_table::user_action_info_t &info) {

    }
    void set_user_actions(const GameId& game_id, const std::vector<ns_table::available_user_course_t> &actions, const ns_table::action_info_t &action_info) {

    }
    void set_users_cashes(const GameId& game_id, const ns_table::users_cashes_t &users_cashes) const {

    }
    void set_winners(const GameId& game_id, const std::pair<std::vector<ns_table::winner_info_t>, std::vector<ns_table::winner_info_t>>& winners
                                                         , const std::vector<std::pair<gbp_i64, std::vector<gbp_i8>>> &users_cards, bool by_combination) const
    {

    }
    void show_tournament_dialog_info(gbp_i64 tournament_id, const ns_table::tournament_dialog_info &info) const {

    }
    void tournament_canceled(        gbp_i64 tournament_id, gbp_i64 buy_in) const {

    }
    void hide_addon(                  const GameId& game_id) const {

    }
    void hide_rebuy_btn(              const GameId& game_id) const {

    }
    void game_deleted(                const GameId& game_id) const {

    }
    void show_rebuy_buttons(          const GameId& game_id, bool double_available) const {

    }
    void show_autobreak_massage(      const GameId& game_id, time_s delay_time) const {

    }
    void show_autobreak_addon_massage(const GameId& game_id, time_s delay_time) const {

    }
    void tournament_break(            const GameId& game_id, time_s break_time) const {

    }
    void tournament_addon_break(      const GameId& game_id, time_s break_time, gbp_i64 addon_money, gbp_i64 addon_chips) const {

    }
    void addon_available(             const GameId& game_id, const gbp::ns_api::addon_t &info, gbp_i64 account_money, time_s time_left) const {

    }
    void bounty(                      const GameId& game_id, const std::vector<std::string> &nicknames, gbp_i64 money) const {

    }
    void open_tournament(             const GameId& game_id, const ns_table::tournament_state_t &tournament_state, const ns_table::table_state_t &table_state, gbp_i64 players_count) const {

    }
    void level_up(                    const GameId& game_id, gbp_i64 big_blind, gbp_i64 ante, qint16 current_round) const {

    }
    void rebuys_waiting(              const GameId& game_id, time_s waiting_time) const {

    }
    void table_change(                const GameId& old_game_id, const ns_table::table_state_t &table_state) const {

    }
};

    rpc_table* getRpc() {
        return rpc_object<rpc_table>();
    }



    template <>
    struct IdGetter<server_request_handler_impl> {
        using id_type = GameId;
        constexpr inline static id_type invalidID() {
            return GameId::invalidID();
        }

        constexpr inline static const id_type& get_id(const server_request_handler_impl& obj) {
            return obj.m_id;
        }
    };


struct HandlerGroup::Data : QSharedData {

    ProxyDataHandler m_dataHandler;
    ProxyViewHandler m_viewHandler;
    ControlsHandler  m_ctrlHandler;

    Data(const GameId& id, QObject *hParent)
        : QSharedData()
        , m_dataHandler(id/*, hParent*/)
        , m_viewHandler(id/*, hParent*/)
        , m_ctrlHandler(id/*, hParent*/)
    {}

    Data(const Data& copied)
        : QSharedData(copied)
        , m_dataHandler(copied.m_dataHandler.id())
        , m_viewHandler(copied.m_viewHandler.id())
        , m_ctrlHandler(copied.m_ctrlHandler.id())
    {}
};

HandlerGroup::HandlerGroup(const GameId& id, QObject *hParent)
    : GameHandlerInterface(id)
    , m_data(new HandlerGroup::Data(id, hParent))
{}

HandlerGroup::~HandlerGroup()
{}

ProxyDataHandler &HandlerGroup::dataHandler() {
    return m_data->m_dataHandler;
}

ProxyViewHandler &HandlerGroup::viewHandler() {
    return m_data->m_viewHandler;
}

ControlsHandler &HandlerGroup::ctrlHandler() {
    return m_data->m_ctrlHandler;
}

void HandlerGroup::initialize()
{
    m_data->m_dataHandler.initialize();
    m_data->m_viewHandler.initialize();
    m_data->m_ctrlHandler.initialize();
}

void HandlerGroup::timelineEvent(const TimeLineEvent &ev)
{
    m_data->m_dataHandler.timelineEvent(ev);
    m_data->m_viewHandler.timelineEvent(ev);
    m_data->m_ctrlHandler.timelineEvent(ev);
}

void HandlerGroup::openTable()
{
    m_data->m_dataHandler.openTable();
    m_data->m_viewHandler.openTable();
    m_data->m_ctrlHandler.openTable();
}

void HandlerGroup::closeTable()
{
    m_data->m_ctrlHandler.closeTable();
    m_data->m_viewHandler.closeTable();
    m_data->m_dataHandler.closeTable();
}








GameManager::GameManager(QObject *parent)
    : QObject(parent)
    , m_client(netClient())
    , m_errHandler(this)
    , m_playersMgr()
    , m_serverRequest(rpc_object<const ns_server_request::emitter_table>())
{

//    connect(&m_playersMgr, &PlayersManager::avatarsUpdated, this, [this](const std::vector<gbp_i64>& ids) {
//        for (gbp_i64 id: ids) {
//            TablePlayerList lst = findPlayersByPlayerId(id);
//            if (!lst.empty()) {
//                QPixmap avatar = m_playersMgr.avatar(id);
//                for (TablePlayer* pl: lst) {
//                    pl->setAvatar(avatar);
//                }
//            }
//        }
//    });


    connect(&m_client, &NetworkClient::loggedIn, this, &GameManager::onLogin);
    connect(&m_client, &NetworkClient::loggedOut, this, &GameManager::onLogout);
    connect(&m_client, &NetworkClient::disconnected, this, &GameManager::onDisconnected);
}

GameManager::~GameManager() {

}

void GameManager::registerServerEvents()
{
//    registerReplyHandlers();
    registerServerRequestHandlers();
}

void GameManager::registerServerRequestHandlers()
{
    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::open_cashes, this
    , [this](const std::vector<table_state_t> &cashes) {
        //qDebug() << "open cashes!";
    });
    //---------------------------------------------------------------------------------------

    //! open_table near the sit_on_table && from_viever_to_player replies

    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::set_table_state, this
    , [this](gbp_i64 id, const ns_table::table_state_t& table_state) {
        dataHandler(id).update(table_state);
    });
    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::set_new_waiting_position, this
    , [this](gbp_i64 id, quint32 waiters_count, quint16 w_pos) {
        // TODO: определить обработчик этого события

    });

    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::show_recharge_message, this
    , [this](gbp_i64 id, const ns_table::recharge_money_t &money) {
        ns_action::CashBuyIn* action = new ns_action::CashBuyIn(id, getGameTable(id), this);
        QSharedPointer<ns_table::recharge_money_t> dataInPtr(new ns_table::recharge_money_t(money));
        action->setInfo(dataInPtr);
        action->setAlreadySitOnTable(true);
        action->execute();
        connect(action, &ns_action::CashBuyIn::done, [&](bool result){
            if (result) {
                //TODO: finish this logic
            } else {

            }
            action->deleteLater();
        });
    });
    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::show_guest_buttons, this
    , [this](gbp_i64 id, bool show) {
        //NOTE: event for controller
        ctrlHandler(id).setProperty("bttn_guest_bet.visible", show);
//        ctrlHandler(id).setProperty("bttn_wait_bb.visible", show);
//        if (CashHandler* game = cashHandler(id)) {
//            if (show) {
//                this->handleFreezeUser(game->id(), game->getPlayer()->playerId());
//            } else {
//                GameAction action = std::move(game->getPlayer()->takeTriggeredAction());
//                if (action.first == e_user_action::guest_bet) {
//                    game->setPlayerAction(game->getPlayer()->playerId(), action);
//                }
//            }
//        }
    });
    //------------------------------------------------------------------------------------

    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::show_hand_buttons, this
    , [this](const GameId& gameID) {
        //NOTE: event for controller
//         if (ProxyDataHandler* game = dataHandler(gameID)) {
// //            game->getPlayer()->showHandButtons();
//         }
    });
    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::show_user_hand, this
    , [this](const GameId& gameID, gbp_i64 user_id, const std::vector<gbp_i8>& cards) {
        //WARNING: event for... view?
    //     if (ProxyDataHandler* game = dataHandler(gameID)) {
    //         game->setPlayerCards(user_id, cards);
    //     }
    });

    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::close_table, this, &GameManager::removeGame);

    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::set_user_actions, this
    , [this](const GameId& gameID, const std::vector<ns_table::available_user_course_t> &actions, const ns_table::action_info_t &action_info) {
        //INFO: event for controller
    //     if (ProxyDataHandler* game = dataHandler(gameID)) {
    //         game->makeUserCurrentPlayer(actions, action_info);
    //     }
    });
    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::set_user_action, this
    , [this](const GameId& gameID, const ns_table::user_action_info_t &info) {
        //INFO: event for model
    //     if (ProxyDataHandler* game = dataHandler(gameID)) {
    //         game->setPlayerAction(info._user_id, std::make_pair(info._action, info._action_cash));
    //     }
    });


    auto lambda_set_common_pot = [&](const GameId& gameID, gbp_i64 pot)
    {

    };

    connect(m_serverRequest, & ns_server_request::table_cash::emitter_t::set_common_pot, lambda_set_common_pot);

    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::clear_common_pot, this
    , [&](const GameId& gameID) {
        lambda_set_common_pot(gameID, 0);
    });
    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::set_predefined_actions, this
    , [this](const GameId& gameID, const ns_table::money_info_t &money_info) {
        //INFO: event for controller (? check if no model data used)
//        if (ProxyDataHandler* game = proxyDataHandler(gameID)) {
//            gbp_i64 currPlayerID = game->getPlayer()->playerId();
//            Q_ASSERT(currPlayerID != -1);

//            game->setPlayerRoundBet(currPlayerID, money_info._user_bet);
//            game->setPlayerDistributionBet(currPlayerID, money_info._user_distr_bet);
//            game->setPlayerCash(currPlayerID, money_info._user_cash);
//            game->updateMoneyInfo(money_info._default_bet, money_info._current_bet);
////            game->getPlayer()->setPredefinedInfo(game->pot(), money_info);
//        }
    });
    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::add_cards, this
    , [this](const GameId& gameID, const gbp::ns_api::distrib_users_cards &users_cards) {
        //INFO: event for model
//         // distrib_users_cards = std::vector<std::pair<gbp_i64, gbp_i8>>
//         if (ProxyDataHandler* game = dataHandler(gameID)) {
//             for (const std::pair<gbp_i64, gbp_i8>& it: users_cards) {
//                 game->addPlayerCard(it.first, it.second);
//             }
// //            // if more than 1 card per user
// //            std::map<gbp_i64, std::vector<gbp_i8>> cards;
// //            for (const std::pair<gbp_i64, gbp_i8>& it: users_cards) {
// //                cards[it.first].push_back(it.second);
// //            }
// //            for (const std::pair<gbp_i64, std::vector<gbp_i8>>& it: cards) {
// //                for (gbp_i8 card: it.second) {
// //                    game->addPlayerCard(it.first, card);
// //                }
// //            }
//             //
// //            for (const std::pair<gbp_i64, gbp_i8>& it: users_cards) {
// //                game->addPlayerCard(it.first, it.second);
// //            }
//         }
    });
    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::add_dealer_cards, this
    , [this](const GameId& gameID, const std::vector<std::pair<gbp_i64, gbp_i8>>& users_cards) {
        //INFO: event for view (? ensure)
    //     if (ProxyDataHandler* game = dataHandler(gameID)) {
    //         for (const std::pair<gbp_i64, gbp_i8>& it: users_cards) {
    //             game->setDealerCard(it.first, it.second);
    //         }
    //     }
    });
    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::set_distribution_number, this
    , [this](const GameId& gameID, gbp_i64 distribution_number) {
        //INFO: event for model
    //     if (ProxyDataHandler* game = dataHandler(gameID)) {
    //         game->startDistribution(distribution_number);
    //     }
    });
    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::set_current_round_type, this
    , [this](const GameId& gameID, gbp::ns_api::ns_table::e_game_round_type round_type) {
        //INFO: event for model
    //     // if (tourn_id != -1) {
    //     //     m_errHandler.handleError(GameErrorHandler::ErrorType::ValidTournamentId, "set_current_round_type", tourn_id);
    //     // }
    //     if (ProxyDataHandler* game = dataHandler(gameID)) {
    //         game->startTradeRound(underlying_cast(round_type));
    //     }
    });
    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::add_table_cards, this
    , [this](const GameId& gameID, const std::vector<gbp_i8> &table_cards) {
        //NOTE: event for model
    //     if (ProxyDataHandler* game = dataHandler(gameID)) {
    //         for (gbp_i8 card: table_cards) {
    //             game->addTableCard(card);
    //         }
    //     }
    });
    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::set_chat_message, this
    , [this](const GameId& gameID, gbp_i64 user_id, const std::string &msg) {
        //WARNING: which handler?
    //     if (ProxyDataHandler* game = dataHandler(gameID)) {
    //         game->setChatMessage(user_id, QString::fromStdString(msg));
    //     }
    });
    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::ask_blind, this
    , [this](const GameId& gameID, bool is_big_blind, gbp_i64 blind) {
        //NOTE: event for controller
    //     if (ProxyDataHandler* game = dataHandler(gameID)) {
    //         game->askBlind(blind, is_big_blind);
    //     }
    });
    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::freeze_user, this, &GameManager::handleFreezeUser);
    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::put_user_in_anticipant, this, &GameManager::handlePutUserInAnticipant);
    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::set_user_online, this, &GameManager::handleSetUserOnline);

    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::add_user, this
    , [this](const GameId& gameID, const ns_table::table_player_info_t &user_info) {
        //NOTE: event for model
//        if (ProxyDataHandler* handler = dataHandler(gameID)) {
////            enshurePlayerCreated(gameID, user_info._id, user_info._nickname.c_str(), sha1_t::fromStdString(user_info._hash_avatar));
//            handler->addPlayer(user_info._id);
//            handler->updatePlayer(user_info);
//            m_playersMgr.requestAvatars();
//        }
    });
    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::remove_users, this
    , [this](const GameId& gameID, const std::vector<gbp_i64> &removed_users) {
        //NOTE: event for model
    //     if (ProxyDataHandler* game = dataHandler(gameID)) {
    //         for (gbp_i64 playerID: removed_users) {
    //             game->removePlayer(playerID);
    //         }
    //     }
    });
    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::from_player_to_viewer, this
    , [this](const GameId& gameID, bool distributions_reason, bool recharge_reason) {
        //NOTE: event for controller
//         if (ProxyDataHandler* game = dataHandler(gameID)) {
// //            if (game->getPlayer()->isSitsOnTable()) {
// //                gbp_i64 stack = game->getPlayer()->gameMoney();
// ////                game->getPlayer()->takeMoney
// //                game->removePlayer(game->getPlayer()->playerId());
// //                game->getPlayer()->toViewer(distributions_reason, recharge_reason);
// //                modelsDB().getUserInfo()->moveMoneyFromIngameMoney(stack);
// //            }
//         }
    });
    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::set_winners, this
    , [this](const GameId& gameID, const gbp::ns_api::ns_table::hi_low_winners &winners, const gbp::ns_api::ns_table::users_and_cards &users_cards, bool by_combination) {
        //WARNING: (? )1. which data have to be updated?
        //         (? )2. how controller handles it
        //     if (ProxyDataHandler* game = dataHandler(gameID)) {
    //         game->setWinners(winners, users_cards, by_combination);
    //     }
    });
    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::set_users_cashes, this
    , [this](const GameId& gameID, const ns_table::users_cashes_t &cashes) {
        //NOTE: event for model
    //     // users_cashes_t = std::vector<user_cashes_item_t>
    //     if (ProxyDataHandler* game = dataHandler(gameID)) {
    //         for (const ns_table::user_cashes_item_t& cashesItem: cashes) {
    //             game->setPlayerCash(cashesItem._user_id, cashesItem._cash);
    //             game->setPlayerRoundBet(cashesItem._user_id, cashesItem._round_bet);
    //         }
    //     }
    });
    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::set_current_user, this
    , [this](const GameId& gameID, gbp_i64 current_user_id, gbp_i64 time_left) {
        dataHandler(gameID).setCurrentPlayer(current_user_id, time_left);
    });
    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::set_pots, this
    , [this](const GameId& gameID, const std::vector<gbp_i64> &pots) {
        dataHandler(gameID).setPots(pots);
    });
    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::set_dealer, this
    , [this](const GameId& gameID, gbp_i64 user_id) {
        dataHandler(gameID).setDealer(user_id);
    });
    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::clear_cards, this
    , [this](const GameId& gameID) {
        ProxyDataHandler& handler = dataHandler(gameID);
        handler.clearTableCards();
        handler.clearPlayersCards();
    });
    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::end_of_trade_round, this
    , [this](const GameId& gameID, gbp_u8 round_number) {
        dataHandler(gameID).endTradeRound(round_number);
    });
    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::end_of_distribution, this
    , [this](const GameId& gameID) {
        dataHandler(gameID).endDistribution();
    });
    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::end_of_game, this
    , [this](const GameId& gameID) {
        dataHandler(gameID).endGame();
    });

    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::set_user_money, this
    , [this](const GameId& gameID, gbp_i64 user_id, gbp_i64 money_) {
        dataHandler(gameID).setPlayerCash(user_id, money_);
    });
}

//void GameManager::registerReplyHandlers()
//{
//    const reply_table_cash::emitter_t* replyEmitter = m_client.emitter<reply_table_cash>();


//    connect(replyEmitter, &ns_table::emitter_t::send_chat_message, this
//    , [this](ns_table::e_status result, const GameId& id)
//    {
//        if (ns_helpers::isOk(result))
//        {
////            if (ProxyDataHandler* game = dataHandler(id)) {
//////                /*game->setChatMessage(game->getPlayer()->playerId(), */game->getPlayer()->takeChatMessage()/*)*/;
////            }
//        }
//        else
//        {
//            m_errHandler.handleError(GameErrorHandler::ErrorType::ReplyStatusNotOk, "send_chat_message", id.m_tableID);
//        }
//    });
//    connect(replyEmitter, &ns_table::emitter_t::close_table, this
//    , [this](ns_table::e_status result, const GameId& gameID) {
//        std::cout << "close_table " << result << " " << gameID.toString().toStdString() << std::endl;
//        if (ns_helpers::isOk(result)) {
//            removeGame(gameID);
//        } else {
//            m_errHandler.handleError(GameErrorHandler::ErrorType::ReplyStatusNotOk, "close_table", gameID.m_tableID);
//        }
//    });
//    connect(replyEmitter, &ns_table::emitter_t::user_action, this
//    , [this](ns_table::e_status status, const GameId& id) {
//        if (ns_helpers::isOk(status)) {
////            if (ProxyDataHandler* game = dataHandler(id)) {

////                auto triggeredAction = std::move(game->getPlayer()->takeTriggeredAction());
////                game->setPlayerAction(game->getPlayer()->playerId(), triggeredAction);
////            }
//        } else {
//            m_errHandler.handleError(GameErrorHandler::ErrorType::ReplyStatusNotOk, "user_action", id.m_tableID, status);
//        }
//    });
//    connect(replyEmitter, &ns_table::emitter_t::user_online, this
//    , [this](ns_table::e_status status, const GameId& id) {
//        if (ns_helpers::isOk(status)) {
//            handleSetUserOnline(id, pApp->client().userId());
//        } else {
//            m_errHandler.handleError(GameErrorHandler::ErrorType::ReplyStatusNotOk, "user_online", id.m_tableID);
//        }
//    });
////    connect(replyEmitter, &ns_table::emitter_t::sit_on_table, this
////    , [this](ns_table::e_status status, gbp_i64 game_id, const ns_table::sit_on_table_info_t &info) {
////        switch (status) {
////            case ns_table::e_status::rat_mode_enabled:
////            case ns_table::e_status::not_enough_money:
////            case ns_table::e_status::ok:
////                onSitOnTableConfirmed(game_id, info);
////                break;
////            default:
////                m_errHandler.handleError(GameErrorHandler::ErrorType::ReplyStatusNotOk, "sit_on_table", game_id, status);
////                break;
////        }
////    });
////    connect(replyEmitter, &ns_table::emitter_t::from_viewer_to_player, this
////    , [this](ns_table::e_status status, gbp_i64 game_id, const ns_table::sit_on_table_info_t &info) {
////        switch (status) {
////            case ns_table::e_status::rat_mode_enabled:
////            case ns_table::e_status::not_enough_money:
////            case ns_table::e_status::ok:
////                onSitOnTableConfirmed(game_id, info);
////                break;
////            default:
////            m_errHandler.handleError(GameErrorHandler::ErrorType::ReplyStatusNotOk, "from_viewer_to_player", game_id, status);
////            break;
////        }
////    });
//    //----
//    connect(m_serverRequest, &ns_server_request::table_cash::emitter_t::open_table, this
//    , [this](gbp_i64 id, const ns_table::sit_on_table_info_t & info, const ns_table::table_state_t &table_state) {

//    });
//    //----

//    connect(replyEmitter, &ns_table::emitter_t::set_sit_money, this
//    , [this](ns_table::e_status status, gbp_i64 game_id, gbp_i64 money_) {
//        if (!ns_helpers::isOk(status)) {
//            m_errHandler.handleError(GameErrorHandler::ErrorType::ReplyStatusNotOk, "set_sit_money", game_id);
//        } else {

//        }
//    });
//    connect(replyEmitter, &ns_table::emitter_t::set_autorecharge, this
//    , [this](ns_table::e_status status, gbp_i64 game_id, bool set) {
//        if (!ns_helpers::isOk(status)) {
//            m_errHandler.handleError(GameErrorHandler::ErrorType::ReplyStatusNotOk, "set_autorecharge", game_id);
//        } else {
////            if (CashHandler* game = cashHandler(game_id)) {
//////                game->setAutorecharge(set);
////            }
//        }
//    });
//    connect(replyEmitter, &ns_table::emitter_t::get_recharge_money, this
//    , [this](ns_table::e_status status, gbp_i64 game_id, const ns_table::recharge_money_t &money) {
//        if (!ns_helpers::isOk(status)) {
//            m_errHandler.handleError(GameErrorHandler::ErrorType::ReplyStatusNotOk, "get_recharge_money", game_id);
//        } else {
////            if (CashHandler* game = cashHandler(game_id))
////            {
////                game->makeBuyIn(money, false);
////            }
//        }
//    });
//    connect(replyEmitter, &ns_table::emitter_t::add_game_money, this
//    , [this](ns_table::e_status status, gbp_i64 game_id) {
////        if (CashHandler* game = cashHandler(game_id))
////        {
////            switch (status) {
////            case ns_table::e_status::ok:
////            case ns_table::e_status::ok_without_waiting:
//////                game->setPlayerCash(game->getPlayer()->playerId(), 2343);
////                break;
////            default:
////                break;
////            }
////        }
//    });
////    connect(replyEmitter, &ns_table::emitter_t::add_viewer, this
////    , [this](ns_table::e_status status, const GameId& id, gbp_i8 /*user_pos*/) {
////        switch (status) {
////        case ns_table::e_status::ok:
////        case ns_table::e_status::exists:
////        case ns_table::e_status::exists_as_player:
////        case ns_table::e_status::exists_as_tournament_player:
////             if (GameHandler* game = gameHandler(id))
////             {
////                 enshureCurrentPlayerCreated(id);
////                 game->openGame(false);
////                 if (TablePlayer* pl = game->findPlayer(game->getPlayer()->playerId())) {
////                     pl->setSeatIndex(-1);
////                 }
////                 // если игрок не залогинен, то объект TablePlayer не добавлен к контролам
////                 // нужно непосредственно обновить
////                 if (game->getPlayer()->playerId() == -1) {
////                    game->getPlayer()->updateGameControls();
////                 }
////             }
////            break;
////        case ns_table::e_status::unknown_error:
////        default:
////            m_errHandler.handleError(GameErrorHandler::ErrorType::ReplyStatusNotOk, "add_viewer", id.m_tableID);
////            break;
////        }
////    });
//    connect(replyEmitter, &ns_table::emitter_t::set_guest_blind, this
//    , [this](ns_table::e_status status, gbp_i64 game_id, bool set) {
//        if (!ns_helpers::isOk(status)) {
//            m_errHandler.handleError(GameErrorHandler::ErrorType::ReplyStatusNotOk, "set_guest_blind", game_id);
//        } else {
////            if (CashHandler* game = cashHandler(game_id))
////            {
//////                game->getPlayer()->setGuestBlindChecked(set);
////            }
//        }
//    });
//    connect(replyEmitter, &ns_table::emitter_t::add_waiter, this
//    , [this](ns_table::e_status status, gbp_i64 game_id, quint16 waiter_number) {
//        if (!ns_helpers::isOk(status)) {
//            m_errHandler.handleError(GameErrorHandler::ErrorType::ReplyStatusNotOk, "add_waiter", game_id);
//        } else {
////            if (CashHandler* game = cashHandler(game_id))
////            {
////                waiter_t* sharedData = pApp->db().sharedDataService().getSingleDataForType<waiter_t>();
////                sharedData->second[game_id] = waiter_number;
//////                game->getPlayer()->setState(E::PlayerState::Waiter, true, "add_waiter");
//////                game->getPlayer()->setWaitersInfo(ns_helpers::waitersCount(game_id), waiter_number);
////                // TODO: уточнить что такое waiter_number, количество ожидающих или номер ожидающего
////                if (game->isOpened()) {
//////                    if (TablePlayer* pl = game->findPlayer(game->getPlayer()->playerId())) {
//////                        pl->setWaitersCount(waiter_number);
//////                        pl->setWaiterIndex(waiter_number);
//////                    }
////                }
////            }
//        }
//    });
//    connect(replyEmitter, &ns_table::emitter_t::remove_from_waiters, this
//    , [this](ns_table::e_status status, gbp_i64 game_id, quint16 waiters_count) {
//        if (!ns_helpers::isOk(status)) {
//            m_errHandler.handleError(GameErrorHandler::ErrorType::ReplyStatusNotOk, "remove_from_waiters", game_id);
//        } else {
////            if (CashHandler* game = cashHandler(game_id))
////            {
////                waiter_t* sharedData = pApp->db().sharedDataService().getSingleDataForType<waiter_t>();
////                sharedData->second.erase(game_id);
////                if (game->isOpened())
////                {
//////                    if (TablePlayer* pl = game->findPlayer(game->getPlayer()->playerId())) {
//////                        pl->setWaitersCount(waiters_count);
//////                        pl->setWaiterIndex(-1);
//////                    }
////                }
////                else
////                {
////                    removeGame(game_id);
////                }
////            }
//        }
//    });
//    connect(replyEmitter, &ns_table::emitter_t::set_sit_out, this
//    , [this](ns_table::e_status status, gbp_i64 game_id, bool set) {
//        if (ns_helpers::isOk(status)) {
////            if (CashHandler* game = cashHandler(game_id)) {
////                game->getPlayer()->setSitOutFromNextDistribution(set);
////            }
//        } else {
//            m_errHandler.handleError(GameErrorHandler::ErrorType::ReplyStatusNotOk, "set_sit_out", game_id);
//        }
//    });
//    connect(replyEmitter, &ns_table::emitter_t::set_show_hand, this
//    , [this](ns_table::e_status status, const GameId& id) {
//        if (!ns_helpers::isOk(status)) {
//            m_errHandler.handleError(GameErrorHandler::ErrorType::ReplyStatusNotOk, "set_show_hand", id.m_tableID);
//        } else {
//            if (ProxyDataHandler* game = dataHandler(id)) {
////                game->getPlayer()->setShowHand();
//            }
//        }
//    });

//}

//TablePlayerList GameManager::findPlayersByPlayerId(gbp_i64 playerID)
//{
//    TablePlayerList players;
////    std::find_if(m_allPlayers.begin(), m_allPlayers.end(), [](TablePlayer* player){});
//    for (TablePlayer* player: m_allPlayers) {
//        if (player->playerID() == playerID) {
//            players.push_back(player);
//        }
//    }
//    return players;
//}

//TablePlayerList GameManager::findPlayersByGameId(const GameId &gameID)
//{
//    if (ProxyDataHandler* proxy = dataHandler(gameID)) {
//        if (GameHandler* game = proxy->handler<GameHandler>()) {
//            return game->tablePlayers();
//        }
//    }
//    return TablePlayerList();
//}

//TablePlayer *GameManager::findTablePlayer(const GamePlayerId &id)
//{
//    TablePlayerList lst = findPlayersByGameId(id.m_gameID);
//    for (TablePlayer* pl: lst) {
//        if (pl->playerID() == id.m_playerID) {
//            return pl;
//        }
//    }
//    return nullptr;
//}


//CashHandler* GameManager::createCashHandler(gbp_i64 id)
//{
//    CashHandler* cashHandler = new CashHandler(id, this);
//    connect(cashHandler, SIGNAL(tableOpened(GameId)), this, SIGNAL(tableOpened(GameId)));
//    connect(cashHandler, &CashHandler::tableOpened, pApp, []{
//        pApp->expandMainLobby(false);
//    });

////    connect(&m_playersMgr, SIGNAL(avatarsUpdated()), cashHandler, SLOT(updatePlayers()));
//    return cashHandler;
//}

GameTable* GameManager::createGameTable(const GameId &gameID, QWidget *parent)
{
    removeGameTable(gameID);

    GameTable* table = new GameTable(gameID, parent);
    table->init();
    m_gameTables.insert(gameID, table);

//    getRpc()->get_table_state(gameID.m_tableID, [this](ns_table::e_status status, const ns_table::table_state_t& tblState){
//        std::cout << "on_GET_TABLE_STATE " << status << " " << tblState << std::endl;
//    });

    return table;
}

void GameManager::removeGameTable(const GameId &gameID)
{
    auto it = m_gameTables.find(gameID);
    if (it != m_gameTables.end()) {
        delete it.value();
        m_gameTables.erase(it);
    }
}

GameTable *GameManager::getGameTable(const GameId &gameID) const
{
    auto it = m_gameTables.find(gameID);
    Q_ASSERT(it != m_gameTables.end());

    return it.value();
}

bool GameManager::isTableCreated(const GameId &gameID) const {
    return m_gameTables.contains(gameID);
}

void GameManager::onLogin()
{
    const gbp::ns_api::ns_lobby::user_info_t *info = modelsDB().getUserInfo()->nativeData();
    Q_ASSERT(info);

//    for (const GameId& id : m_handlers.uniqueKeys()) {
//        TablePlayer* pl = TablePlayer::create(info->_user_id, id, info->_nickname.c_str(), sha1_t::fromStdString(info->_hash_avatar), QLocale::Country(info->_country));
//        m_allPlayers.insert(pl->id(), pl);
//        m_handlers.value(id).first->addPlayer(pl->id().m_playerID);
//        if (auto* player = m_handlers.value(id).first->getPlayer()) {
//            player->onLogin(pl);
//        }
//    }

}

void GameManager::onLogout(long long playerID)
{
    Q_ASSERT(!modelsDB().getUserInfo()->nativeData());

    for (HandlerGroup& handlers: m_handlers) {
        handlers.ctrlHandler().onLogout(playerID);
    }
}

void GameManager::onDisconnected()
{
    if (!m_handlers.empty())
    {
        for (const GameId& id: m_handlers.uniqueKeys()) {
            removeGame(id);
        }
    }
}

HandlerGroup& GameManager::handlers(const GameId& id)
{
    QMap<GameId, HandlerGroup>::iterator it = m_handlers.find(id);
    if (it == m_handlers.end()) {
        it = m_handlers.insert(id, HandlerGroup(id, this));
    }
    return it.value();
}

ProxyDataHandler &GameManager::dataHandler(const GameId &id) {
    return handlers(id).dataHandler();
}
ProxyViewHandler &GameManager::viewHandler(const GameId &id) {
    return handlers(id).viewHandler();
}
ControlsHandler &GameManager::ctrlHandler(const GameId &id) {
    return handlers(id).ctrlHandler();
}

//GameHandler *GameManager::gameHandler(const GameId& id) const
//{
//    if (ProxyDataHandler* proxy = proxyDataHandler(id)) {
//        return proxy->handler<GameHandler>();
//    }
//    return nullptr;
//}
//CashHandler *GameManager::cashHandler(gbp_i64 id) const
//{
//    GameId gameID(id, -1);
//    if (ProxyDataHandler* proxy = proxyDataHandler(gameID)) {
//        return proxy->handler<CashHandler>();
//    }
//    return nullptr;
//}

bool GameManager::initGame(const GameId& gameId, QWidget *viewParent)
{
//    if ( handlers() ) {
//        return false;
//    }

    GameTable* gameTable = createGameTable(gameId, viewParent);

    HandlerGroup& group = handlers(gameId);
    TableDataUpdater* tableUpdater = new TableDataUpdater(gameId, this);
    TableSurfaceViewHandler* tableSurfaceHandler = new TableSurfaceViewHandler(gameId, this);

    group.dataHandler().push_back(tableUpdater);
    group.viewHandler().push_back(tableSurfaceHandler);
    group.ctrlHandler().setHud(gameTable->hud());
#ifdef ENABLE_VISUAL_DEBUGGING
    DebugInfoPrinterGameDataHandler* dataPrinter = new DebugInfoPrinterGameDataHandler(gameId);
    DebugInfoPrinterGameViewHandler* viewPrinter = new DebugInfoPrinterGameViewHandler(gameId, this);
    group.dataHandler().push_back(dataPrinter);
    group.viewHandler().push_back(viewPrinter);
    dataPrinter->enableDebug(false);
    dataPrinter->enableLog(true);
    viewPrinter->enableDebug(false);
    viewPrinter->enableLog(true);
#endif //ENABLE_VISUAL_DEBUGGING
    gameTable->layers()->setModelKeys();
    tableSurfaceHandler->assign(gameTable);
    group.initialize();

    return true;
}

void GameManager::removeGame(const GameId &gameId)
{
    m_gameTables.value(gameId)->closeTable();
    auto it = m_handlers.find(gameId);
    if (it != m_handlers.end()) {
        it.value().closeTable();
        m_handlers.erase(it);
    }

    emit tableClosed(gameId);
    if (m_handlers.empty()) {
        pApp->expandMainLobby();
    }
    removeGameTable(gameId);
}

//bool GameManager::requestSitOnTable(gbp_i64 gameId, gbp_i8 pos)
//{
//    if (/*CashHandler* game =*/ cashHandler(gameId) != nullptr) {
//        getRpc()->sit_on_table(gameId, pos);
//        return true;
//    }
//    return false;
//}

//bool GameManager::requestWatchTheGame(gbp_i64 gameId)
//{
////    if (!getGameTable(gameId)) {
////        createGameTable(gameId);
////    }
//    getRpc()->add_viewer(gameId, [this, gameId](ns_table::e_status status){
//        switch (status) {
//        case ns_table::e_status::ok:
//        case ns_table::e_status::exists:
//        case ns_table::e_status::exists_as_player:
//        case ns_table::e_status::exists_as_tournament_player:
////             if (game)
//             {

//                 auto* table = getGameTable(gameId);
//                 table->open();

//                 {QDialog(table).open();}

////                 enshureCurrentPlayerCreated(game->id());
////                 game->openGame(false);
////                     if (TablePlayer* pl = game->findPlayer(game->getPlayer()->playerId())) {
////                         pl->setSeatIndex(-1);
////                     }
////                     // если игрок не залогинен, то объект TablePlayer не добавлен к контролам
////                     // нужно непосредственно обновить
////                     if (game->getPlayer()->playerId() == -1) {
////                        game->getPlayer()->updateGameControls();
////                     }
//             }
//            break;
//        case ns_table::e_status::unknown_error:
//        default:
//            m_errHandler.handleError(GameErrorHandler::ErrorType::ReplyStatusNotOk, "add_viewer", gameId);
//            break;
//        }
//    });
//    return true;
//}

//bool GameManager::requestJoinQueue(gbp_i64 gameId)
//{
//    if (/*CashHandler* game =*/ cashHandler(gameId) != nullptr) {
//        getRpc()->add_waiter(gameId);
//        return true;
//    }
//    return false;
//}

//bool GameManager::requestLeaveQueue(gbp_i64 gameId)
//{
//    if (/*CashHandler* game =*/ cashHandler(gameId) != nullptr) {
//        getRpc()->remove_from_waiters(gameId);
//        return true;
//    }
//    return false;
//}

//void GameManager::onSitOnTableConfirmed(const GameId& gameId, const ns_table::sit_on_table_info_t &info)
//{
//    if (ProxyDataHandler* handler = proxyDataHandler(gameId))
//    {
//        auto userInfo = modelsDB().getUserInfo()->nativeData();
//        enshureCurrentPlayerCreated(gameId);
//        handler->addPlayer(userInfo->_user_id);
//        handler->getPlayer()->setViewer(false);
//        handler->setPlayerPos(userInfo->_user_id, info._pos);
//        handler->openTable();
//        if (CashHandler* cashHandler = handler->handler<CashHandler>()) {
//            cashHandler->makeBuyIn(info._money, true, info._rat_time);
//        }
//    }
//}

//void GameManager::enshurePlayerCreated(const GameId &gameID, gbp_i64 playerID, const QString& nickname, const sha1_t& hashAvatar)
//{
//    GamePlayerId gamePlayerID(gameID, playerID);
//    TablePlayer* player = findTablePlayer(gamePlayerID);
//    if (player == nullptr) {
//        player = TablePlayer::create(gamePlayerID, nickname, hashAvatar);
//        m_playersMgr.addPlayer(playerID, nickname, hashAvatar);
//        m_allPlayers.insert(player->id(), player);
//        if (GameHandler* handler = gameHandler(gameID)) {
//            handler->playersGroup()->addAction(player);
//        }
//    }
//}

//void GameManager::enshureCurrentPlayerCreated(const GameId &gameID)
//{
//    gbp_i64 playerID = netClient().userId();
//    if (playerID != -1)
//    {
//        GamePlayerId gamePlayerID(gameID, playerID);
//        TablePlayer* player = findTablePlayer(gamePlayerID);
//        if (player == nullptr)
//        {
//            auto userInfo = modelsDB().getUserInfo()->nativeData();

//            auto nick = QString::fromStdString(userInfo->_nickname);
//            auto hash = sha1_t::fromStdString(userInfo->_hash_avatar);
//            auto locl = QLocale::Country(userInfo->_country);
//            player = TablePlayer::create(gamePlayerID, nick, hash, locl);
//            m_playersMgr.addPlayer(playerID, nick, hash, locl);
//            if (ProxyDataHandler* handler = proxyDataHandler(gameID)) {
////                handler->playersGroup()->addAction(player);
//                if (auto* pl = handler->getPlayer()) {
//                    pl->setTablePlayer(player);
//                }
//            }
//        }
//        if (!m_allPlayers.contains(gamePlayerID))
//        {
//            m_allPlayers.insert(gamePlayerID, player);
//        }

//    }
//}

void GameManager::handleFreezeUser(const GameId &gameID, gbp_i64 user_id)
{
    auto& handler = dataHandler(gameID);
    ns_table::user_state_t state = qvariant_cast<ns_table::user_state_t>(handler.userState(user_id));
    state._frozen = true;
    handler.setUserState(user_id, state);

}
void GameManager::handlePutUserInAnticipant(const GameId &gameID, gbp_i64 user_id, bool /*return_to_game_btn*/)
{
    auto& handler = dataHandler(gameID);
    ns_table::user_state_t state = qvariant_cast<ns_table::user_state_t>(handler.userState(user_id));
    state._anticipant = true;
    handler.setUserState(user_id, state);
//    if (ProxyDataHandler* handler = dataHandler(gameID)) {
//    }
//    GamePlayerId gamePlayerID(gameID, user_id);
//    if (TablePlayer* pl = findTablePlayer(gamePlayerID)) {
//        pl->sitOut();
//    }
//    if (ProxyHandler* game = proxyHandler(gameID)) {
//        game->getPlayer()->getControl("bttn_backToGame")->setEnabled(return_to_game_btn);
//    }
}
void GameManager::handleSetUserOnline(const GameId &gameID, gbp_i64 user_id)
{
    auto& handler = dataHandler(gameID);
    ns_table::user_state_t state = qvariant_cast<ns_table::user_state_t>(handler.userState(user_id));
    state._anticipant = false;
    handler.setUserState(user_id, state);

//    GamePlayerId gamePlayerID(gameID, user_id);
//    if (TablePlayer* pl = findTablePlayer(gamePlayerID)) {
//        pl->setOnline();
//    }
//    if (GameHandler* game = gameHandler(gameID)) {
//        game->setPlayerState(user_id, GameHandler::PlayerState_Frozen);
//    }
}
