#pragma once

#include "gamehandlerinterface.hpp"
#include <tools/types_fwd.hpp>
#include <api_models/defs.hpp>

namespace ns_model {
    class table_state_t;
    class CommonGameTable;
} //namespace ns_model

class TableDataUpdater : public QObject, public GameDataHandlerInterface
{
    Q_OBJECT
private:
    struct TableDataUpdaterPrivate;
    TableDataUpdaterPrivate* m_impl;
public:
    explicit TableDataUpdater(const GameId& gameId, QObject *parent = nullptr);
    virtual ~TableDataUpdater();
    ns_model::table_state_t* model() const;
protected:
    virtual void timelineEvent(const TimeLineEvent &ev) override;
public slots:
    virtual void updatePlayers() override;

public:
    virtual void initialize() override;
    virtual gbp_i64 pot(int index) const;
    virtual gbp_i64 bigBlind() const;
    virtual QString tableName() const;
    virtual e_game_type gameType() const;
    virtual e_game_limit gameLimit() const;
    virtual void update(const table_state_t &tableState) override;
    virtual void clearPots() override;
    virtual void setPots(const std::vector<gbp_i64> &pots) override;
    virtual void setPot(gbp_i64 money, gbp_i8 idx) override;
    virtual void updateMoneyInfo(gbp_i64 defaultBet, gbp_i64 currentBet) override;
    virtual void clearTableCards() override;
    virtual void addTableCard(gbp_i8 card) override;
    virtual void setTableCard(gbp_i8 card, gbp_i8 idx) override;
    virtual void clearPlayersCards() override;
    virtual void addPlayerCard(gbp_i64 playerID, gbp_i8 card) override;
    virtual void setPlayerCards(gbp_i64 playerID, const std::vector<gbp_i8> &cards) override;
    virtual void setDealerCard(gbp_i64 playerID, gbp_i8 card) override;
    virtual void setPlayerPos(gbp_i64 playerID, gbp_i8 pos) override;
    virtual void setPlayerAction(gbp_i64 playerID, const GameAction &action) override;
    virtual void setPlayerCash(gbp_i64 playerID, gbp_i64 money) override;
    virtual void setPlayerRoundBet(gbp_i64 playerID, gbp_i64 money) override;
    virtual void setPlayerDistributionBet(gbp_i64 playerID, gbp_i64 money) override;
    virtual void setDealer(gbp_i64 playerID) override;
    virtual void setCurrentPlayer(gbp_i64 playerID, gbp_i64 timeLeft, gbp_i64 timePassed) override;
    virtual void makeUserCurrentPlayer(const std::vector<available_user_course_t> &actions, const action_info_t &action_info) override;
    virtual void askBlind(gbp_i64 blind, bool isBB) const override;
    virtual void addPlayer(gbp_i64 playerID) override;
    virtual void removePlayer(gbp_i64 playerID) override;
    virtual void updatePlayer(const table_player_info_t &info) override;
    virtual void startGame() override;
    virtual void endGame() override;
    virtual void startDistribution(gbp_i64 n) override;
    virtual void endDistribution() override;
    virtual void startTradeRound(gbp_i8 n) override;
    virtual void endTradeRound(gbp_i8 n) override;
    virtual void setWinners(const std::pair<std::vector<winner_info_t>, std::vector<winner_info_t> > &winners, const std::vector<std::pair<gbp_i64, std::vector<gbp_i8> > > &users_cards, bool by_combination) override;
    virtual void setChatMessage(gbp_i64 playerID, const QString &msg) override;
    virtual void setUserState(gbp_i64 playerID, const gbp::ns_api::ns_table::user_state_t &state) override;
    virtual QVariant userState(gbp_i64 playerID) const override;
protected:
    virtual void onFreeSeatsCountChanged() override;

public slots:
    virtual void openTable() override;
    virtual void closeTable() override;
};

