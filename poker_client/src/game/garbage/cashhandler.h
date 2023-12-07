#pragma once

#include "tabledataupdater.h"
#include "player.h"

#include <core/money.h>
#include <qabstractbutton.h>
#include <interface/identifiable.h>

class GameTable;
struct CashHandlerData;

class CashHandler : public GameHandler
{
    Q_OBJECT
    using SuperClass = GameHandler;
private:
    QScopedPointer<CashHandlerData> m_dPtr;
public:
    CashHandler(gbp_i64 id, QObject* parent = nullptr);
    virtual ~CashHandler();

public slots:
    virtual void updatePlayers();
public:
    virtual void initialize() override;
    virtual void update(const table_state_t& tableState) override;
    virtual void clearPots() override;
    virtual void setPots(const std::vector<gbp_i64> &pots) override;
    virtual void setPot(gbp_i64 money, gbp_i8 idx) override;

    virtual void updateMoneyInfo(gbp_i64 defaultBet, gbp_i64 currentBet) override;

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
    virtual void askBlind(gbp_i64 blind, bool isBB) const override;
    virtual void setCurrentPlayer(gbp_i64 playerID, gbp_i64 timeLeft, gbp_i64 timePassed = 0) override;
    virtual void setDealer(gbp_i64 playerID) override;
    virtual void startGame() override;
    virtual void endGame() override;
    virtual void startDistribution(gbp_i64 n) override;
    virtual void endDistribution() override;
    virtual void startTradeRound(gbp_i8 n) override;
    virtual void endTradeRound(gbp_i8 n) override;
    virtual void setWinners( const std::pair<std::vector<winner_info_t>, std::vector<winner_info_t> > &winners
                           , const std::vector<std::pair<gbp_i64, std::vector<gbp_i8> > > &users_cards
                           , bool by_combination) override;
    virtual void setChatMessage(gbp_i64 playerID, const QString &msg) override;
protected:
    virtual void timelineEvent(const TimeLineEvent& ev) override;
    virtual void onFreeSeatsCountChanged() override;
public:
    void makeBuyIn(const recharge_money_t &money, bool beforeSit = true, gbp_i64 ratTime = -1);
    void setWinner(const winner_info_t& winner, bool showCombination);
    void chatPlayerAction(gbp_i64 playerID, const QString& actionName, Money actionMoney = 0);

    virtual gbp_i64 pot(int index = 0) const override;
    virtual gbp_i64 bigBlind() const override;
    virtual QString tableName() const override;
    virtual e_game_type gameType() const override;
    virtual e_game_limit gameLimit() const override;
    virtual QVariant userState(gbp_i64 playerID) const override;

    virtual void openTable() override;
    virtual void closeTable() override;
protected slots:
    virtual void onModelReset() /*override*/;
    virtual void onModelDataChanged(const QModelIndexRange& range) /*override*/;
    virtual void onModelAboutToInsertItems(const QModelIndex &parent) /*override*/;
    virtual void onModelInsertItems(const QModelIndexRangeList &items) /*override*/;
    virtual void onModelAboutToRemoveItems(const QModelIndexRangeList &items) /*override*/;
    virtual void onModelRemoveItems(const QModelIndex &parent) /*override*/;

    // GameHandlerInterface interface
public:
    virtual void setUserState(gbp_i64 playerID, const gbp::ns_api::ns_table::user_state_t& state) override;
};
