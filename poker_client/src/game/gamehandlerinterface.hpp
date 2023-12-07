#pragma once

#include <tools/types_fwd.hpp>
#include <interface/identifiable.h>

class QAction;
class QAbstractButton;
class TimeLineEvent;

namespace
{
    using money_info_t            = gbp::ns_api::ns_table::money_info_t;
    using action_info_t           = gbp::ns_api::ns_table::action_info_t;
    using user_action_info_t      = gbp::ns_api::ns_table::user_action_info_t;
    using e_user_action           = gbp::ns_api::ns_table::e_user_action;
    using available_user_course_t = gbp::ns_api::ns_table::available_user_course_t;
    using table_player_info_t     = gbp::ns_api::ns_table::table_player_info_t;
    using user_state_t            = gbp::ns_api::ns_table::user_state_t;
    using winner_info_t           = gbp::ns_api::ns_table::winner_info_t;
    using table_state_t           = gbp::ns_api::ns_table::table_state_t;
    using recharge_money_t        = gbp::ns_api::ns_table::recharge_money_t;
    using e_game_type             = gbp::ns_api::ns_game::e_game_type;
    using e_game_limit            = gbp::ns_api::ns_game::e_game_limit;
}

//! @brief Base handler interface
class GameHandlerInterface : public GameIdObject
{
public:
    GameHandlerInterface(const GameId& id)
        : GameIdObject(id)
    {}
    virtual ~GameHandlerInterface() {}
    virtual void initialize() = 0;
    virtual void timelineEvent(const TimeLineEvent &ev) = 0;
    virtual void openTable() = 0;
    virtual void closeTable() = 0;
};

//! @brief Handles table data changes, set data to 'item model'
class GameDataHandlerInterface : public GameHandlerInterface
{
public:
    GameDataHandlerInterface(const GameId& id)
        : GameHandlerInterface(id)
    {}
    virtual ~GameDataHandlerInterface() {}
public:
    virtual void updatePlayers() = 0;
    virtual void update(const ns_table::table_state_t &tableState) = 0;
    virtual void clearPots() = 0;
    virtual void setPots(const std::vector<gbp_i64> &pots) = 0;
    virtual void setPot(gbp_i64 money, gbp_i8 idx) = 0;
    virtual void updateMoneyInfo(gbp_i64 defaultBet, gbp_i64 currentBet) = 0;
    virtual void clearTableCards() = 0;
    virtual void addTableCard(gbp_i8 card) = 0;
    virtual void setTableCard(gbp_i8 card, gbp_i8 idx) = 0;
    virtual void clearPlayersCards() = 0;
    virtual void addPlayerCard(gbp_i64 playerID, gbp_i8 card) = 0;
    virtual void setPlayerCards(gbp_i64 playerID, const std::vector<gbp_i8> &cards) = 0;
    virtual void setDealerCard(gbp_i64 playerID, gbp_i8 card) = 0;
    virtual void setPlayerPos(gbp_i64 playerID, gbp_i8 pos) = 0;
    virtual void setPlayerAction(gbp_i64 playerID, const GameAction &action) = 0;
    virtual void setPlayerCash(gbp_i64 playerID, gbp_i64 money) = 0;
    virtual void setPlayerRoundBet(gbp_i64 playerID, gbp_i64 money) = 0;
    virtual void setPlayerDistributionBet(gbp_i64 playerID, gbp_i64 money) = 0;
    virtual void setDealer(gbp_i64 playerID) = 0;
    virtual void setCurrentPlayer(gbp_i64 playerID, gbp_i64 timeLeft, gbp_i64 timePassed = 0) = 0;
    virtual void makeUserCurrentPlayer(const std::vector<available_user_course_t> &actions, const action_info_t &action_info) = 0;
    virtual void askBlind(gbp_i64 blind, bool isBB) const = 0;
    virtual void startGame() = 0;
    virtual void endGame() = 0;
    virtual void startDistribution(gbp_i64 n) = 0;
    virtual void endDistribution() = 0;
    virtual void startTradeRound(gbp_i8 n) = 0;
    virtual void endTradeRound(gbp_i8 n) = 0;
    virtual void setWinners(const std::pair<std::vector<winner_info_t>, std::vector<winner_info_t> > &winners, const std::vector<std::pair<gbp_i64, std::vector<gbp_i8> > > &users_cards, bool by_combination) = 0;
    virtual void setChatMessage(gbp_i64 playerID, const QString &msg) = 0;
    virtual void addPlayer(gbp_i64 playerID) = 0;
    virtual void removePlayer(gbp_i64 playerID) = 0;
    virtual void updatePlayer(const table_player_info_t& info) = 0;
    virtual void onFreeSeatsCountChanged() = 0;

    virtual void setUserState(gbp_i64 playerID, const gbp::ns_api::ns_table::user_state_t& state) = 0;
    virtual QVariant userState(gbp_i64 playerID) const = 0;
};

//! @brief Handles wherever model data changed, notify widgets and views
class GameViewHandlerInterface : public GameHandlerInterface
{
public:
    GameViewHandlerInterface(const GameId& id)
        : GameHandlerInterface(id)
    {}
    virtual ~GameViewHandlerInterface() {}

    virtual void onModelAboutToBeReset() = 0;
    virtual void onModelReset() = 0;
    virtual void onModelDataChanged(const QModelIndexRange& range) = 0;
    virtual void onModelAboutToInsertItems(const QModelIndex& parent) = 0;
    virtual void onModelInsertItems(const QModelIndexRangeList& items) = 0;
    virtual void onModelAboutToRemoveItems(const QModelIndexRangeList& items) = 0;
    virtual void onModelRemoveItems(const QModelIndex& parent) = 0;
    virtual bool indexAffectsView(const QModelIndex& index) const = 0;

    virtual void setIndexWidget(QWidget* widget, const QModelIndex& index = QModelIndex()) = 0;
    virtual QWidget* indexWidget(const QModelIndex& index = QModelIndex()) const = 0;
protected:
    friend class ProxyViewHandler;
    virtual void connectToModel() = 0;
    virtual void disconnectFromModel() = 0;
};

//! @brief Handles available user actions, changes visual controls visibility, activity ect..
class ControlsHandlerInterface : public GameHandlerInterface
{
public:
    ControlsHandlerInterface(const GameId& id)
        : GameHandlerInterface(id) {}
    virtual ~ControlsHandlerInterface() {}

    virtual void onLogout(gbp_i64 playerID) = 0;
    virtual void onLogin(gbp_i64 playerID) = 0;

    // например аватар, или место в очереди
    virtual void onModelDataChanged(const QModelIndex& index) = 0;
public:
    virtual void onActionTriggered(QAction*) = 0;
    virtual void onActionToggled(QAction*, bool) = 0;
    virtual void onButtonClicked(QAbstractButton*) {}
    virtual void onButtonToggled(QAbstractButton*, bool) {}
};
