#pragma once

#include "gamehandlerinterface.hpp"
#include "proxyhandler.h"

class ProxyDataHandler : public ProxyHandler<GameDataHandlerInterface>
{
public:
    ProxyDataHandler(const GameId& id);
    virtual ~ProxyDataHandler();

public:
    virtual void initialize() override final;
    virtual void updatePlayers() override final;
    virtual void update(const table_state_t &tableState) override final;
    virtual void clearPots() override final;
    virtual void setPots(const std::vector<gbp_i64> &pots) override final;
    virtual void setPot(gbp_i64 money, gbp_i8 idx) override final;
    virtual void updateMoneyInfo(gbp_i64 defaultBet, gbp_i64 currentBet) override final;
    virtual void clearTableCards() override final;
    virtual void addTableCard(gbp_i8 card) override final;
    virtual void setTableCard(gbp_i8 card, gbp_i8 idx) override final;
    virtual void clearPlayersCards() override final;
    virtual void addPlayerCard(gbp_i64 playerID, gbp_i8 card) override final;
    virtual void setPlayerCards(gbp_i64 playerID, const std::vector<gbp_i8> &cards) override final;
    virtual void setDealerCard(gbp_i64 playerID, gbp_i8 card) override final;
    virtual void setPlayerPos(gbp_i64 playerID, gbp_i8 pos) override final;
    virtual void setPlayerAction(gbp_i64 playerID, const GameAction &action) override final;
    virtual void setPlayerCash(gbp_i64 playerID, gbp_i64 money) override final;
    virtual void setPlayerRoundBet(gbp_i64 playerID, gbp_i64 money) override final;
    virtual void setPlayerDistributionBet(gbp_i64 playerID, gbp_i64 money) override final;
    virtual void setDealer(gbp_i64 playerID) override final;
    virtual void setCurrentPlayer(gbp_i64 playerID, gbp_i64 timeLeft, gbp_i64 timePassed = 0) override final;
    virtual void makeUserCurrentPlayer(const std::vector<available_user_course_t> &actions, const action_info_t &action_info) override final;
    virtual void askBlind(gbp_i64 blind, bool isBB) const override final;
    virtual void startGame() override final;
    virtual void endGame() override final;
    virtual void startDistribution(gbp_i64 n) override final;
    virtual void endDistribution() override final;
    virtual void startTradeRound(gbp_i8 n) override final;
    virtual void endTradeRound(gbp_i8 n) override final;
    virtual void setWinners(const std::pair<std::vector<winner_info_t>, std::vector<winner_info_t> > &winners, const std::vector<std::pair<gbp_i64, std::vector<gbp_i8> > > &users_cards, bool by_combination) override final;
    virtual void setChatMessage(gbp_i64 playerID, const QString &msg) override final;
    virtual void addPlayer(gbp_i64 playerID) override final;
    virtual void removePlayer(gbp_i64 playerID) override final;
    virtual void updatePlayer(const table_player_info_t &info) override final;

    virtual void setUserState(gbp_i64 playerID, const gbp::ns_api::ns_table::user_state_t &state) override;
    virtual QVariant userState(gbp_i64 playerID) const override;
    virtual void timelineEvent(const TimeLineEvent &ev) override final;
protected:
    virtual void onFreeSeatsCountChanged() override final;
public slots:
    virtual void openTable() override;
    virtual void closeTable() override;
};
