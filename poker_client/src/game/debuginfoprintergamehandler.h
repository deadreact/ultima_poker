#pragma once
#include "abstractgamehandler.h"

namespace ns_model {
class LogModel;
} //namespace ns_model

struct Debug
{
    const bool& m_enabled;
    Debug(const bool &enabled);
    Debug& contexual(const char* file, int line, const char* msg);
    template <typename T>
    Debug& operator <<(const T& smth) {
        if (m_enabled) {
            qDebug() << smth;
        }
        return *this;
    }
};


//! @class DebugInfoPrinterGameHandler
class DebugInfoPrinter
{
protected:
    bool m_enableLog;
    bool m_enableDebug;

    virtual QString scope() const = 0;
public:
    DebugInfoPrinter();

    virtual ns_model::LogModel &log(const char *func, const char* file, int line, const QStringList &args = QStringList()) const;
    virtual Debug &debug(const char* func, const char* file, int line, const QStringList &args = QStringList()) const;
    ns_model::LogModel &log(const char *func, const QStringList &args = QStringList()) const;
    Debug &debug(const char* func, const QStringList &args = QStringList()) const;

    inline void enableLog(bool enable = true) {
        m_enableLog = enable;
    }

    inline void enableDebug(bool enable = true) {
        m_enableDebug = enable;
    }
};

//! @class DebugInfoPrinterGameDataHandler
//! @inherits DebugInfoPrinter, GameDataHandlerInterface
class DebugInfoPrinterGameDataHandler : public DebugInfoPrinter, public GameDataHandlerInterface
{
protected:
    virtual QString scope() const override;
public:
    DebugInfoPrinterGameDataHandler(const GameId& id);

    virtual void initialize() override;
    virtual void updatePlayers() override;
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
    virtual void askBlind(gbp_i64 blind, bool isBB) const override;
    virtual void startGame() override;
    virtual void endGame() override;
    virtual void startDistribution(gbp_i64 n) override;
    virtual void endDistribution() override;
    virtual void startTradeRound(gbp_i8 n) override;
    virtual void endTradeRound(gbp_i8 n) override;
    virtual void setWinners(const std::pair<std::vector<winner_info_t>, std::vector<winner_info_t> > &winners, const std::vector<std::pair<gbp_i64, std::vector<gbp_i8> > > &users_cards, bool by_combination) override;
    virtual void setChatMessage(gbp_i64 playerID, const QString &msg) override;
    virtual void addPlayer(gbp_i64 playerID) override;
    virtual void removePlayer(gbp_i64 playerID) override;
    virtual void updatePlayer(const table_player_info_t &info) override;
    virtual void onFreeSeatsCountChanged() override;
    virtual void timelineEvent(const TimeLineEvent &ev) override;
    virtual void setUserState(gbp_i64 playerID, const gbp::ns_api::ns_table::user_state_t& state) override;
    virtual QVariant userState(gbp_i64 playerID) const override;

    // GameHandlerInterface interface
public:
    virtual void makeUserCurrentPlayer(const std::vector<available_user_course_t> &actions, const action_info_t &action_info) override;

    // GameHandlerInterface interface
public slots:
    virtual void openTable() override;
    virtual void closeTable() override;
};


//! @class DebugInfoPrinterGameViewHandler
//! @inherits DebugInfoPrinter, GameViewHandlerInterface
class DebugInfoPrinterGameViewHandler : public GameViewHandler, public DebugInfoPrinter
{
protected:
    virtual QString scope() const override;
public:
    DebugInfoPrinterGameViewHandler(const GameId& gameID, QObject* parent = nullptr);

    virtual ns_model::LogModel &log(const char *func, const char* file, int line, const QStringList &args = QStringList()) const override;
public:
    virtual void initialize() override;
    virtual void timelineEvent(const TimeLineEvent &ev) override;

    virtual void onModelAboutToBeReset() override;
    virtual void onModelReset() override;
    virtual void onModelDataChanged(const QModelIndexRange &range) override;
    virtual void onModelAboutToInsertItems(const QModelIndex &parent) override;
    virtual void onModelInsertItems(const QModelIndexRangeList &items) override;
    virtual void onModelAboutToRemoveItems(const QModelIndexRangeList &items) override;
    virtual void onModelRemoveItems(const QModelIndex &parent) override;
    virtual bool indexAffectsView(const QModelIndex &index) const override;
protected:
    virtual void connectToModel() override;
    virtual void disconnectFromModel() override;
public:
    virtual void openTable() override;
    virtual void closeTable() override;
};
